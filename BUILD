package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

load("@build_bazel_rules_apple//apple:swift.bzl", "swift_library")

OBJC_COPTS = [
    "-Werror",
    "-Wextra",
    "-Wall",
    "-Wstrict-prototypes",
    "-Wdocumentation",
]

SWIFT_COPTS = [
    "-wmo",
]

objc_library(
    name = "ResumableAssert",
    srcs = glob([
        "Sources/ResumableAssert/*.m",
    ]),
    hdrs = glob([
        "Sources/ResumableAssert/include/*.h",
    ]) + [
        "ResumableAssert.h",
    ],
    copts = OBJC_COPTS,
    includes = [
        "Sources/ResumableAssert/include",
    ],
    module_map = "Sources/ResumableAssert/include/module.modulemap",
)

swift_library(
    name = "ResumableAssertSwift",
    srcs = glob([
        "Sources/ResumableAssert/*.swift",
    ]),
    copts = SWIFT_COPTS + [
        "-import-underlying-module",
    ],
    module_name = "ResumableAssert",
    swift_version = 4,
    deps = [
        ":ResumableAssert",
    ],
)
