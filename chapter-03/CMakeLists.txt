
add_executable(counting-team-members counting-team-members/main.cpp)
add_executable(older-than-generic    older-than-generic/main.cpp)

set_property(TARGET counting-team-members PROPERTY FOLDER "examples/chapter-03")
set_property(TARGET older-than-generic    PROPERTY FOLDER "examples/chapter-03")

