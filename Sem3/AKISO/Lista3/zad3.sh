#!/bin/bash
# Ustawienia API
CAT_API_URL="https://api.thecatapi.com/v1/images/search"
CHUCK_NORRIS_API_URL="https://api.chucknorris.io/jokes/random"
echo "Pobieranie obrazka kota..."
cat_image_url=$(curl -s "$CAT_API_URL" | jq -r '.[0].url')
echo "Pobieranie cytatu Chucka Norrisa..."
chuck_norris_joke=$(curl -s "$CHUCK_NORRIS_API_URL" | jq -r '.value')

echo "Wyświetlanie obrazka kota:"
curl -s "$cat_image_url" -o /tmp/cat_image.jpg | catimg /tmp/cat_image.jpg
    
echo -e "\nLosowy cytat Chucka Norrisa:"
echo "$chuck_norris_joke"
