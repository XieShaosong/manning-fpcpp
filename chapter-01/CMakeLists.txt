
add_executable(count-lines-stdcount  count-lines-stdcount/main.cpp  )
add_executable(count-lines-transform count-lines-transform/main.cpp )

set_property(TARGET count-lines-stdcount   PROPERTY FOLDER "examples/chapter-01")
set_property(TARGET count-lines-transform  PROPERTY FOLDER "examples/chapter-01")
