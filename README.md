# projet-integration-salma
Système d'Information Embarqué pour éviter l'oubli de passagers lors d'excursions.

**Composants** : Arduino Uno + RFID MFRC522 + LCD I2C + HC-SR04 + LEDs + Buzzer  
**Simulation** : Wokwi | **Projet Intégration** 2024-2025
## Simulation rapide sur Wokwi (sans installation)

1. Ouvrir le lien : https://wokwi.com/projects/459328021117409281
2. Cliquer sur le bouton **Run** (triangle vert)
3. Cliquer sur une **carte RFID** pour simuler un scan de badge
4. Observer le LCD, les LEDs et le buzzer en temps réel

## Comportements à tester

| Action dans Wokwi | Résultat attendu |
|---|---|
| Clic sur une carte RFID | LCD : `ACCES VALIDE / SIEGE No: N` + LED verte |
| 30 scans successifs | LCD : `BUS EST PLEIN !` + LED rouge permanente |
| 31ème scan | LCD : `BUS COMPLET ! / STOP EMBARQUER` + buzzer grave |
| Distance HC-SR04 < 50cm (bus plein) | LCD : `ATTENTE PORTE ! / Degagez l'entree` |
| Appui bouton RESET | LCD : `REINITIALISATION / Bus vide...` → compteur = 0 |
## Déploiement sur Arduino physique (IDE Arduino)

### Étape 1 — Cloner le dépôt
```
git clone https://github.com/salmakhili8-ji/projet-integration-salma
```

### Étape 2 — Installer les bibliothèques dans Arduino IDE
- Ouvrir Arduino IDE → **Outils** → **Gérer les bibliothèques**
- Chercher `MFRC522` → installer 
- Chercher `LiquidCrystal I2C` 

### Étape 3 — Téléverser le code
- Ouvrir `main.ino` dans Arduino IDE
- **Outils** → **Carte** → **Arduino Uno**
- **Outils** → **Port** → sélectionner votre port COM
- Cliquer sur **Téléverser** (Ctrl+U)



## Câblage physique

| Composant | Broche Arduino |
|---|---|
| RFID SS | D10 |
| RFID RST | D9 |
| RFID MOSI | D11 |
| RFID MISO | D12 |
| RFID SCK | D13 |
| RFID VCC | **3.3V** (pas 5V !) |
| HC-SR04 TRIG | D4 |
| HC-SR04 ECHO | D3 |
| LCD SDA | A4 |
| LCD SCL | A5 |
| LED verte | D6 (via 220Ω) |
| LED rouge | D5 (via 220Ω) |
| Buzzer | D7 |
| Bouton RESET | D8 (INPUT_PULLUP) |



## Modifier le seuil de capacité

Dans `main.ino`, ligne 17 :
const int SEUIL_MAX = 30;  // Changer selon la capacité du véhicule
