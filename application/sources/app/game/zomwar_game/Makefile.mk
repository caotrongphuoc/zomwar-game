CFLAGS		+= -I./sources/app/game/zomwar_game
CPPFLAGS	+= -I./sources/app/game/zomwar_game

VPATH += sources/app/game/zomwar_game

# CPP source files 
# Zomwar 
SOURCES_CPP += sources/app/game/zomwar_game/ar_game_zomwar.cpp
SOURCES_CPP += sources/app/game/zomwar_game/ar_game_bullet.cpp
SOURCES_CPP += sources/app/game/zomwar_game/ar_game_bang.cpp
SOURCES_CPP += sources/app/game/zomwar_game/ar_game_border.cpp
SOURCES_CPP += sources/app/game/zomwar_game/ar_game_zombie.cpp
SOURCES_CPP += sources/app/game/zomwar_game/ar_game_car.cpp