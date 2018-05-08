# name of your application
APPLICATION = octa-demo

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= ../../

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps # debug threads
USEMODULE += saul_default
USEMODULE += printf_float
USEMODULE += xtimer

USEMODULE += fireflyx1
USEMODULE += oss7modem

include $(RIOTBASE)/Makefile.include
