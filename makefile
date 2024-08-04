# ----------------------------
# Makefile Options
# ----------------------------

NAME = amines
DESCRIPTION = "Ag C minesweeper"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
