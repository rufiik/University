# Rafal Wochna 279752
# x : wektor współczynników x
# y : wektor współczynników y
# where T : typ zmiennoprzecinkowy (np. Float32)
function ForwardSum(x::Vector{T}, y::Vector{T})::T where T
    n = 5
    result = zero(T)
    for i in 1:n
        result += x[i] * y[i]
    end
    return result
end
function BackwardSum(x::Vector{T}, y::Vector{T})::T where T
    n = 5
    result = zero(T)
    for i in n:-1:1
        result += x[i] * y[i]
    end
    return result
end
function HighestToLowestSum(x::Vector{T}, y::Vector{T})::T where T
    n = 5
    products = Vector{T}(undef, n)
    for i in 1:n
        products[i] = x[i] * y[i]
    end
    positive_products = filter(p -> p > zero(T), products)
    negative_products = filter(p -> p < zero(T), products)
    
    sort!(positive_products, rev=true)
    
    sort!(negative_products)
    
    positive_sum = zero(T)
    for p in positive_products
        positive_sum += p
    end
    
    negative_sum = zero(T)
    for n in negative_products
        negative_sum += n
    end
    
    return positive_sum + negative_sum
end
function LowestToHighestSum(x::Vector{T}, y::Vector{T})::T where T
    n = 5
    products = Vector{T}(undef, n)
    for i in 1:n
        products[i] = x[i] * y[i]
    end
    positive_products = filter(p -> p > zero(T), products)
    negative_products = filter(p -> p < zero(T), products)
    
    sort!(positive_products)
    
    sort!(negative_products, rev=true)
    
    positive_sum = zero(T)
    for p in positive_products
        positive_sum += p
    end
    
    negative_sum = zero(T)
    for n in negative_products
        negative_sum += n
    end
    
    return positive_sum + negative_sum
end
# Dane do testów i wyświetlanie wyników
xfl32 = [Float32(2.718281828), Float32(−3.141592654), Float32(1.414213562), Float32(0.5772156649), Float32(0.3010299957)]
yfl32 = [Float32(1486.2497), Float32(878366.9879), Float32(−22.37492), Float32(4773714.647), Float32(0.000185049)]

xfl64 = [Float64(2.718281828), Float64(−3.141592654), Float64(1.414213562), Float64(0.5772156649), Float64(0.3010299957)]
yfl64 = [Float64(1486.2497), Float64(878366.9879), Float64(−22.37492), Float64(4773714.647), Float64(0.000185049)]

println("Float32:")
println("Forward Sum: ", ForwardSum(xfl32, yfl32))
println("Backward Sum: ", BackwardSum(xfl32, yfl32))
println("Highest to Lowest Sum: ", HighestToLowestSum(xfl32, yfl32))
println("Lowest to Highest Sum: ", LowestToHighestSum(xfl32, yfl32))
println("\nFloat64:")
println("Forward Sum: ", ForwardSum(xfl64, yfl64))
println("Backward Sum: ", BackwardSum(xfl64, yfl64))
println("Highest to Lowest Sum: ", HighestToLowestSum(xfl64, yfl64))
println("Lowest to Highest Sum: ", LowestToHighestSum(xfl64, yfl64))