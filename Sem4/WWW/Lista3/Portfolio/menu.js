/*jslint browser:true */
document.addEventListener("DOMContentLoaded", function () {
    const menuToggle = document.querySelector(".menu-toggle");
    const navMenu = document.querySelector("header nav ul");

    // Obsługa kliknięcia w przycisk menu hamburgerowego
    menuToggle.addEventListener("click", function () {
        navMenu.classList.toggle("active"); 
    });

    // Obsługa zdarzeń hover dla dużych ekranów
    const menuItems = document.querySelectorAll("header nav ul li");
    menuItems.forEach(function (item) {
        item.addEventListener("mouseenter", function () {
            item.classList.add("hovered"); 
        });

        item.addEventListener("mouseleave", function () {
            item.classList.remove("hovered"); 
        });
    });
});