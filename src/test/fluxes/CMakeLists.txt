function(add a0 a1 a2)
  set(name fluxes-${a0}-${a1}-${a2})
  add_test(NAME ${name} COMMAND run ${a0} ${a1} ${a2} ${ARGN})
  set_tests_properties(${name} PROPERTIES RESOURCE_LOCK fluxes)
endfunction()

add(simple imp gravity)
add(simple exp gravity)
add(proj imp gravity)
add(proj exp gravity)
