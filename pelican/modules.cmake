add_subdirectory(utility)  # depends on: !nothing!

add_subdirectory(emulator) # depends on: utility
add_subdirectory(data)     # depends on: utility

add_subdirectory(modules)  # depends on: data, utility

add_subdirectory(comms)
add_subdirectory(server)
add_subdirectory(output)
add_subdirectory(core)
add_subdirectory(viewer)
add_subdirectory(examples)

add_subdirectory(doc)
