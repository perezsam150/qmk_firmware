# Matrix configuration
CUSTOM_MATRIX = lite
SRC += matrix.c

# Optimize firmware
LTO_ENABLE = yes
VIA_ENABLE = yes

# Disable unused features
MOUSEKEY_ENABLE = no
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
RGBLIGHT_ENABLE = no
TAP_DANCE_ENABLE = no

# Optional features
NKRO_ENABLE = yes
BOOTMAGIC_ENABLE = yes
UNICODEMAP_ENABLE = yes
ENCODER_MAP_ENABLE = yes

# RGB Matrix config
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DEFAULT_MODE = RGB_MATRIX_SOLID_COLOR
