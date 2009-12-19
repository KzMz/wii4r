require 'mkmf'

name = "wii4r"

dir_config(name)
have_library("wiiuse", "wiiuse_init")
create_makefile(name)
