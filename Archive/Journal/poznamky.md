v diplomce nakreslit vysledny diagram, ale v textu by melo byt zduvodneni
v technicke dokumentace by mohla byt historie vyvoje
ESP32 by nemela byt halvni kapitola v diplomce (cca 25%, 10 stranek)
Neuronky jsou vic dulezite


Pro technickou dokumentace: 
- softwarova dokumentace (wiki)
- usecase diagram


# How to combine SW and HW?

1. Train the SW NN to detect a person based on its features (features will determine NN). Just use the dataset of persons photos and train the model to detect each person separately.
2. Use markers on people, like QR code or like.
3. Choose a person on the frame, make a boundary and then let the algorithm follow that person
4. We can check if video distance is correct using hw distance