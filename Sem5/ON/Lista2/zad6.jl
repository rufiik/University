#Rafal Wochna 279752
using Plots
# Funkcja rekurencyjna x_{n+1} = x_n^2 + c
function xsquared_rec(x::Float64, c::Float64)
    next_x::Float64 = x * x + c
    return next_x
end

# Parametry c, x0
configs = [
    (-2.0, 1.0),              
    (-2.0, 2.0),               
    (-2.0, 1.99999999999999),  
    (-1.0, 1.0),              
    (-1.0, -1.0),             
    (-1.0, 0.75),           
    (-1.0, 0.25)               
]
for i in 1:7
        global c, x0 = configs[i]
        println("Wyniki dla c = $(c) i x0 = $(x0):")
    for n in 1:40
        global x0 = xsquared_rec(x0, c)
        println("$(x0)")
    end

end

