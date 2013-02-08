
# Pelican modules
add_subdirectory(utility)  # DEPS: !nothing!
add_subdirectory(emulator) # DEPS: utility
add_subdirectory(data)     # DEPS: utility
add_subdirectory(comms)    # DEPS: data, utility
add_subdirectory(server)   # DEPS: comms, utility
                           # DEPS(tests): server, emulator, data, comms,
                           #   utility, emulatorTest
add_subdirectory(output)   # DEPS: comms, data, server
                           # DEPS(test): dataTestUtils, serverTestUtils,
                           #   utilityTestUtils
add_subdirectory(core)     # DEPS: server, output, comms, data
                           # DEPS(test): core, data, utility, serverTestUtils,
                           #   emulator, dataTestUtils, utilityTestUtils
add_subdirectory(viewer)   # DEPS: output, comms, utility
                           # DEPS(test): viewer, outputTestUtils


# Pelican examples
add_subdirectory(examples) # DEPS: core, server, emulator

#add_subdirectory(doc)
