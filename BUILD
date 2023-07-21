load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
)

package(default_visibility = ["//visibility:public"])

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)

sh_test(
    name = "envoy_binary_test",
    srcs = ["envoy_binary_test.sh"],
    data = [":envoy"],
)