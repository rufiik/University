/*jslint browser:true */
document.addEventListener("DOMContentLoaded", function () {
    const galleryContainer = document.getElementById("gallery-container");
    const imageUrls = [
        "public/kot1.jpg",
        "public/kot2.jpg",
        "public/kot3.jpg",
        "public/kot4.jpg",
        "public/kot5.jpg"
    ];

    // Funkcja do ładowania pojedynczego obrazu
    function loadImage(url) {
        return new Promise(function (resolve, reject) {
            const img = new window.Image(); 
            img.src = url;
            img.onload = function () {
                resolve(img); // Obraz załadowany
            };
            img.onerror = function () {
                reject(new Error(
                    "Nie udało się załadować obrazu: " + url
                ));
            };
        });
    }

    // Ładowanie wszystkich obrazów równolegle
    Promise.all(imageUrls.map(function (url) {
        return loadImage(url);
    }))
        .then(function (images) {
            // Dodaj załadowane obrazy do kontenera galerii
            images.forEach(function (img) {
                galleryContainer.appendChild(img);
            });
        })
        .catch(function (error) {
            console.error("Błąd podczas ładowania obrazów:", error);
        });
});