# CanSat 2023 repository

# Pokyny pro ovládání senzorů

- Funkce get_sensors_status po dotazu na /sensors_status očekává odpověď ve formátu
```json
[
    {
        "sensor": "<sensor_name>",
        "value": "<sensor_value>"
    }
]
```

- Po volbě stavu senzoru je odeslán dotaz na / s parametry ve tvaru
```json
{
    "sensor": "<sensor_name>",
    "value": "<sensor_value>"
}
```
