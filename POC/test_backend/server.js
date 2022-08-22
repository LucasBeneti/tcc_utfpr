const express = require("express");

const app = express();

const PORT = 3000;

app.get("/test", (req, res) => {
  res.send("Hello world!");
});

/*
o que estamos esperando em termos de dados e como que iremos guardar?

dados:{
    time: <date and hour together?>,
    umidade: number
    temperatura: number
    carbonDioxide: number
    luminosidade: true | false
}

no header acho que é uma boa mandar um token ou algo assim pra gente ter certeza que é o 
device autorizado pra esse registro...mas pra essa primeira POC isso não será tão necessário
*/

app.listen(PORT);
