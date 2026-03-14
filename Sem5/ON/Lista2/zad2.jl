#Rafal Wochna 279752
using Plots
# Definicja funkcji
f(x) = exp(x) * log(1 + exp(-x))
# Zakres wartości x
x = -10:0.01:55
y = f.(x)
# Rysowanie wykresu
p = plot(x, y, label="e^x * ln(1 + e^(-x))", xlabel="x", ylabel="y", title="Wykres funkcji", legend=:top)
xlims!(-10, 55)  
ylims!(0, 3)    

# Zapisanie wykresu do pliku PNG
savefig(p, "wykres_funkcji.png")