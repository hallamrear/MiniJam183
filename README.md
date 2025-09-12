# Mini Jam 183 - Calculations / Four Colours

## TODO:
- Potentially full main menu screen
    - Intro
    - Attribution
- Create an actual ui system to properly scale buttons relative to screen size
    - Include a proper button system to make things more responsive and cleanup the 
- Rework the card system. I want it to have a limited card range (0-10 with 0 & 10 being super rare) where you sacrifice smaller cards to create larger ones. For example: you can break 5, +, and 8 cards down to create a 13. Also, I'd like to add the ability to sacrifice division cards to undo this and get the original cards back.
- Implement Enemy Definition Parenting
- Move and properly store the enemy definitions after loading.

## Bugs:
- nullptr checks in render. 
- Enemy scaling
- Make avatars bigger and move the health bar.
- Remove so many point checks in BattleScene::Update
- Game hangs when you beat the final boss. It goes back to the map and you can't do anything.11
- Move number and operand card textures into a single spritesheet.
    - Potentially move them into TTF text *things* rather than textures.
- Intro screen currently just restarts until the mouse is pressed.

## Assets
- [Kenney's Game Assets All-in-1](https://kenney.itch.io/kenney-game-assets)
- [Pixel BG's (Map BG)](https://space-spheremaps.itch.io/pixelart-starfields)
- [Daniel Linssen's m6x11 font](https://managore.itch.io/m6x11)
- [Jesse Munguia's Skeleton Sprite Pack](https://jesse-m.itch.io/skeleton-pack)
- [Shikashi's Fantasy Icon Pack](https://shikashipx.itch.io/shikashis-fantasy-icons-pack)