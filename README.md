# Wokwi Project - Collecte de Données Météorologiques

Ce projet utilise un microcontrôleur pour collecter des données météorologiques, telles que la température, l'humidité et les mouvements. Les données sont ensuite envoyées à ThingsBoard pour une visualisation et une analyse en temps réel. Ce système peut être utilisé pour des projets éducatifs ou des installations à petite échelle.

## Fonctionnalités

- **Mesure de la température et de l'humidité** : Utilisation d'un capteur DHT22 pour lire les données météorologiques.
- **Détection de mouvement** : Un capteur PIR détecte les mouvements et allume une LED lorsqu'un mouvement est détecté.
- **Affichage OLED** : Affichage des messages sur un écran OLED en fonction des données collectées.
- **Envoi de données à ThingsBoard** : Les données collectées sont envoyées à ThingsBoard pour analyse.

## Matériel Utilisé

- Microcontrôleur ESP32
- Capteur de température et d'humidité DHT22
- Capteur de mouvement PIR
- LED
- Écran OLED (128x64)
- Connexion WiFi
