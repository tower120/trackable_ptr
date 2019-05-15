_add_test(unique_trackable unique_trackable.cpp)
_add_test(trackable trackable.cpp)

# v2
_add_test(v2.unique_trackable v2/unique_trackable.cpp)
_add_test(v2.trackable v2/trackable.cpp)
_add_test(v2.trackable_base v2/trackable_base.cpp)
_add_test(v2.constness v2/constness.cpp)

SET(constness_failure_msg "T must be const!")
_add_failure_test(v2.constness_failure.ctr_trackable v2/constness_failure/ctr_trackable.cpp constness_failure_msg)
_add_failure_test(v2.constness_failure.ctr_trackable_base v2/constness_failure/ctr_trackable_base.cpp constness_failure_msg)
_add_failure_test(v2.constness_failure.assign_trackable v2/constness_failure/assign_trackable.cpp constness_failure_msg)
_add_failure_test(v2.constness_failure.assign_trackable_base v2/constness_failure/assign_trackable_base.cpp constness_failure_msg)
