package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

OBJC_COPTS = [
    "-Werror",
    "-Wextra",
    "-Wall",
    "-Wstrict-prototypes",
    "-Wdocumentation",
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
