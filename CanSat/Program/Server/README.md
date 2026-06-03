# Pokyny pro ovládání senzorů

- Funkce get_sensors po dotazu na /sensors očekává odpověď ve formátu
```json
[
    {
        "sensor": "<sensor_name>",
        "value": "<sensor_value>"
    }
]
```
Senzory, které chceme pouze zapnout a vypnout, musí mít nastavenou hodnotu `sensor_value` na "on", nebo "off"

- Po volbě stavu senzoru je odeslán dotaz na / s parametry ve tvaru
```json
{
    "sensor": "<sensor_name>",
    "value": "<sensor_value>"
}
```