# NIRFAS GPS Tracker

![NIRFAS GPS Tracker](Project%20Images/image-1.jpeg)

## About the Project

**NIRFAS GPS Tracker** lets you see the live location of your GPS tracking device directly on your phone!  
You can easily attach the device to anything you want to track — a vehicle, a backpack, or even a pet — and watch its movement in real-time on a map through our custom mobile app.


Under the hood, it’s powered by an **ESP32** microcontroller connected to a **NEO-6M GPS module**.  
The device sends its current position to a **Firebase Realtime Database** every 5 seconds via Wi-Fi, and the mobile app reads this data and shows the latest location on an **OpenStreetMap** powered map.

---

## Features

- 📡 Real-time GPS tracking
- 🔥 Seamless integration with Firebase Realtime Database
- 🛰️ ESP32 + NEO-6M GPS Module hardware setup
- 🌍 Mobile app with OpenStreetMap integration
- 🔄 Automatic data refresh every 5 seconds
- 📱 Simple and user-friendly mobile app

---

## How It Works

1. **ESP32 + GPS Module**  
   The ESP32 reads location coordinates (latitude and longitude) from the NEO-6M GPS module.

2. **Data Transmission**  
   Every 5 seconds, the ESP32 sends the latest location data to the Firebase Realtime Database over Wi-Fi.

3. **Mobile App**  
   The mobile app fetches location data from Firebase every 5 seconds and displays the device's live position on an OpenStreetMap map.

---

## Team Behind the Project

- **Oasimul Hasan Raj** (Myself)
- **Hasan Rahmed Sani**
- **Abu Ahmad Alomgir**
- **Fariha Zafar Niha**

---


## License

This project is built for educational and personal project purposes.
