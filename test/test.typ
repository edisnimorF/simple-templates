#import "../srcs/render.typ": render-code-file, render-style

#show: render-style.with()

= Test

#render-code-file("tests/test.cpp")
