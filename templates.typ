#import "srcs/render.typ": render-code-file, render-style

#show: render-style.with()

= Test

#render-code-file("others/default.cpp")

= Calc Geometry

#render-code-file("geo/convex_hull_2d.cpp")

#render-code-file("geo/convex_hull_3d.cpp")
