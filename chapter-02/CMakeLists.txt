
add_executable(average-score                  average-score/main.cpp)
add_executable(count-lines-using-accumulate   count-lines-using-accumulate/main.cpp)
add_executable(filter-and-transform           filter-and-transform/main.cpp)
add_executable(filter-and-transform-combined  filter-and-transform-combined/main.cpp)
add_executable(filtering-using-remove-if      filtering-using-remove-if/main.cpp)
add_executable(move-selected                  move-selected/main.cpp)


set_property(TARGET average-score                 PROPERTY FOLDER "examples/chapter-02")
set_property(TARGET count-lines-using-accumulate  PROPERTY FOLDER "examples/chapter-02")
set_property(TARGET filter-and-transform          PROPERTY FOLDER "examples/chapter-02")
set_property(TARGET filter-and-transform-combined PROPERTY FOLDER "examples/chapter-02")
set_property(TARGET filtering-using-remove-if     PROPERTY FOLDER "examples/chapter-02")
set_property(TARGET move-selected                 PROPERTY FOLDER "examples/chapter-02")





