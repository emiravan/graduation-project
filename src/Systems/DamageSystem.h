#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
   public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        Entity a = event.a;
        Entity b = event.b;
        Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

        // --- Projectile Logic (Existing) ---

        if (a.BelongsToGroup("projectiles") && b.HasTag("player")) {
            OnProjectileHitsPlayer(a, b);
        }

        if (b.BelongsToGroup("projectiles") && a.HasTag("player")) {
            OnProjectileHitsPlayer(b, a);
        }

        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) {
            OnProjectileHitsEnemy(a, b);
        }

        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) {
            OnProjectileHitsEnemy(b, a);
        }

        // --- NEW LOGIC: Player vs Enemies ---

        if (a.HasTag("player") && b.BelongsToGroup("enemies")) {
            OnPlayerHitsDanger(a, b);  // a is player, b is enemy
        }

        if (b.HasTag("player") && a.BelongsToGroup("enemies")) {
            OnPlayerHitsDanger(b, a);  // b is player, a is enemy
        }

        // --- NEW LOGIC: Player vs Obstacles (Trees) ---

        if (a.HasTag("player") && b.BelongsToGroup("obstacles")) {
            OnPlayerHitsDanger(a, b);  // a is player, b is tree
        }

        if (b.HasTag("player") && a.BelongsToGroup("obstacles")) {
            OnPlayerHitsDanger(b, a);  // b is player, a is tree
        }
    }

    void OnProjectileHitsPlayer(Entity projectile, Entity player) {
        const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly) {
            auto& health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentDamage;

            if (health.healthPercentage <= 0) {
                player.Kill();
            }
            projectile.Kill();
        }
    }

    void OnProjectileHitsEnemy(Entity projectile, Entity enemy) {
        const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (projectileComponent.isFriendly) {
            auto& health = enemy.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentDamage;

            if (health.healthPercentage <= 0) {
                enemy.Kill();
            }
            projectile.Kill();
        }
    }

    // New helper function to handle instant death collisions
    void OnPlayerHitsDanger(Entity player, Entity danger) {
        // Ensure the player actually has health before accessing it
        if (player.HasComponent<HealthComponent>()) {
            auto& health = player.GetComponent<HealthComponent>();

            // Subtract 100 health as requested for instant kill
            health.healthPercentage -= 100;

            if (health.healthPercentage <= 0) {
                player.Kill();
            }
        }
    }
};

#endif