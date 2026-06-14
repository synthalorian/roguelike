# SCOPE: ROGUELIKE

## v1 — The Dungeon

- [ ] Map generation: BSP (binary space partitioning) or cellular automata
- [ ] Player entity: `@` on the map, movement, collision
- [ ] Field of View: recursive shadowcasting
- [ ] Simple enemies: static `g` (goblin), `o` (orc), basic AI (hunt player if in FOV)
- [ ] Combat: bump attack, HP, death
- [ ] Game over screen + permadeath (restart from floor 1)
- [ ] Stairs to descend deeper

## v2 — Items & Depth

- [ ] Inventory system: list of items, carry capacity
- [ ] Items: potions (heal), scrolls, weapons, armor
- [ ] Equipment slots: weapon, armor, ring
- [ ] Deeper dungeon levels: harder enemies, better loot
- [ ] Enemy variety: ranged, pack behavior, fleeing at low HP
- [ ] Scoring: gold collected, floors reached, enemies killed

## v3 — Persistence & Content

- [ ] Save/load (single slot, deleted on death)
- [ ] More enemy types: undead, demons, dragons
- [ ] Status effects: poison, blind, slow, berserk
- [ ] Boss floors every 5 levels
- [ ] Content complete: 20+ enemy types, 30+ items, 10+ dungeon themes

## Architecture

```
roguelike/
├── src/
│   ├── main.c             # entry, game loop
│   ├── game.c/h           # game state, turn management
│   ├── map.c/h            # grid, terrain, FOV, generation
│   ├── entity.c/h         # ECS-lite: entities, components
│   ├── player.c/h         # player-specific logic
│   ├── enemy.c/h          # AI routines
│   ├── combat.c/h         # damage, HP, death
│   ├── item.c/h           # item definitions, effects
│   ├── inventory.c/h      # inventory UI + logic
│   ├── render.c/h         # ncurses draw calls
│   └── input.c/h          # key handling
├── include/               # public headers
├── levels/                # room templates (optional)
├── Makefile
└── (build.zig for Zig port)
```

### Entity System (ECS-lite)

- `Entity` = index into component arrays
- `Position` array: `x, y, map_id` per entity
- `Stats` array: `hp, max_hp, attack, defense, speed`
- `AI` array: `type, state, target`
- `Item` array: `type, effect, quantity` (only for entities that are items)
- No archetype ECS. Simple parallel arrays for clarity.

### Game Loop

```
Input → Update (player + all enemies) → FOV recalc → Render → Repeat
```

## Milestones

| Day | Target |
|-----|--------|
| 1 | Walk around a generated dungeon |
| 3 | Combat, enemies, death, restart |
| 7 | Full dungeon: items, inventory, stairs, scoring |
| 14 | Content complete: variety, balance, polish |

## Deferred / Future

- Tileset graphics (SDL2 or raylib)
- Mouse support + UI panels
- Magic system (spells, mana)
- Multi-floor persistent maps
- Seed sharing ("try seed 0xBEEF")
