# ROGUELIKE

> A traditional ASCII roguelike. Permadeath. Procedural dungeons. Pure terminal.

## What Is This?

A canonical roguelike in the tradition of Rogue, NetHack, and Dungeon Crawl Stone Soup. ASCII graphics, turn-based combat, procedural dungeon generation, and permadeath. Every run is different. Every death is final.

## Genre Notes

- **Permadeath**: No save scumming. One life, one dungeon.
- **Procedural**: Floors, enemies, and items are generated anew each run.
- **FOV**: Shadowcasting for realistic line-of-sight. You only see what you can see.
- **Turn-Based**: Time moves when you move. Every action matters.
- **ASCII**: `@` is the hero. `g` is a goblin. `!` is a potion. Imagination is the GPU.

## Stack

- **C** (v1) or **Zig** (v2) — tradition vs. modern safety
- **ncurses** — terminal UI, colors, input handling
- Custom fallback terminal output for minimal dependency builds

## How to Build

```bash
cd roguelike
# C + ncurses
make
./roguelike

# Zig (optional)
zig build
zig-out/bin/roguelike
```

## Controls

| Key | Action |
|-----|--------|
| `h/j/k/l` or arrows | Move |
| `>` | Descend stairs |
| `g` | Pick up item |
| `i` | Inventory |
| `d` | Drop item |
| `u` | Use / quaff |
| `?` | Help |
| `Q` | Quit / abandon run |

## Screenshot

```
[screenshot_placeholder: dungeon_fov.png]
```

## License

MIT
