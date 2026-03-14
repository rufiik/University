#Rafal Wochna 279752
# Model logistyczny populacji dla różnych precyzji zmiennoprzecinkowych
function population(p::Float32, r::Float32)
    next_p::Float32 = p + r * p * (1 - p)
    return next_p
end

function population(p::Float64, r::Float64)
    next_p::Float64 = p + r * p * (1 - p)
    return next_p
end

rFL32 = Float32(3.0)
rFL64 = Float64(3.0)
pFL32 = Float32(0.01)
pFL64 = Float64(0.01)

for n in 1:10
    global pFL32 = population(pFL32, rFL32)
    global pFL64 = population(pFL64, rFL64)
    println("   $(n)   |  $(pFL32)|  $(pFL32) | $(pFL64)")
end

pFL32_truncate = floor(pFL32; digits=3)

for n in 11:40
    global pFL32 = population(pFL32, rFL32)
    global pFL64 = population(pFL64, rFL64)
    global pFL32_truncate = population(pFL32_truncate, rFL32)
    println("   $(n)    | $(pFL32) | $(pFL32_truncate) | $(pFL64)")
end

