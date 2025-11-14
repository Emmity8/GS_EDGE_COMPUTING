# 🏢 **Sistema Inteligente de Bem-Estar no Trabalho**

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-Wokwi_Simulation-blue?style=for-the-badge&logo=arduino)
![IoT](https://img.shields.io/badge/IoT-FIWARE_Platform-green?style=for-the-badge&logo=iot)
![MQTT](https://img.shields.io/badge/Protocol-MQTT-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Funcionando-success?style=for-the-badge)

**Monitoramento ambiental inteligente para home office**

[Problema](#-problema) • [Solução](#-solução) • [Funcionalidades](#-funcionalidades) • [Tecnologias](#-tecnologias) • [Demonstração](#-demonstração) • [Instalação](#-instalação)

</div>

## 🎯 **Problema**

> No ambiente de trabalho remoto, perdemos a percepção das condições ambientais que afetam diretamente nossa saúde e produtividade:

- 🌡️ **Temperaturas inadequadas** causando desconforto térmico
- 💧 **Umidade desregulada** afetando as vias respiratórias  
- 💡 **Iluminação inadequada** forçando a visão e causando fadiga
- ⏰ **Longas jornadas** sem pausas levando a LER/DORT

**Impacto:** Redução de até 20% na produtividade e problemas de saúde a longo prazo

## 💡 **Solução**

Desenvolvemos um **sistema IoT inteligente** que atua como um assistente de bem-estar digital, monitorando o ambiente de trabalho em tempo real e fornecendo alertas visuais quando as condições saem do ideal.

### 🚦 **Sistema de Semáforo Inteligente**

| LED | Status | Significado | Ação Recomendada |
|-----|--------|-------------|------------------|
| 🟢 **VERDE** | Condições ideais | Ambiente perfeito para produtividade | Manter as condições atuais |
| 🟡 **AMARELO** | Atenção | Condições abaixo do ideal | Ajustes recomendados |
| 🔴 **VERMELHO** | Alerta crítico | Condições prejudiciais à saúde | Ação imediata necessária |

## ⚡ **Funcionalidades**

### 🎯 **Monitoramento em Tempo Real**
- ✅ **Temperatura ambiente** (DHT22) - Faixa ideal: 20-26°C
- ✅ **Umidade relativa do ar** (DHT22) - Faixa ideal: 40-60%  
- ✅ **Nível de luminosidade** (LDR) - Faixa ideal: 40-80%
- ✅ **Publicação MQTT** para plataforma FIWARE

### 🔔 **Sistema de Alertas Inteligente**
- ✅ **LED Verde** - Todas as condições dentro da faixa ideal
- ✅ **LED Amarelo** - Condições fora do ideal (ajustes necessários)
- ✅ **LED Vermelho** - Condições críticas (ação imediata)
- ✅ **Pausas programadas** - Sugestões a cada 30 minutos

### 🌐 **Integração com Ecossistema IoT**
- ✅ **Conexão WiFi** automática
- ✅ **Comunicação MQTT** com broker FIWARE
- ✅ **Dados em tempo real** no tópico `/TEF/device007/attrs`

## 🛠 **Tecnologias**

| Componente | Função | Especificações |
|------------|--------|----------------|
| **ESP32** | Microcontrolador | WiFi + Processamento |
| **DHT22** | Sensor Ambiente | Temperatura + Umidade |
| **LDR** | Sensor Luminosidade | Fotoresistor |
| **LEDs RGB** | Interface Visual | Verde/Amarelo/Vermelho |
| **MQTT** | Protocolo IoT | Broker: 44.223.43.74:1883 |
| **FIWARE** | Plataforma IoT | Context Broker |

## 🎬 **Demonstração**

### 📊 **Dados no Serial Monitor**
```
=== 📊 DADOS AMBIENTAIS ===
🌡️ Temperatura: 23.5°C
💧 Umidade: 45.2%  
💡 Luminosidade: 67%
✅ Condições ideais!
📤 Dados publicados: t|23.5|u|45.2|l|67|s|on
```

### 🎯 **Cenários de Teste**

| Condição | Ação no Wokwi | Resultado Esperado |
|----------|---------------|-------------------|
| **🌡️ Temperatura Alta** | DHT22 → Temperature: 32°C | 🔴 LED Vermelho + Alerta |
| **💡 Ambiente Escuro** | LDR → lightLevel: 15 | 🔴 LED Vermelho + Alerta |  
| **✅ Condições Ideais** | DHT22: 23°C, 50% + LDR: 60% | 🟢 LED Verde |
| **⏰ Pausa Automática** | Aguardar 30 segundos | 🟡 LED Amarelo Piscante |

### ⚙ **Vista do Circuito Montado**

![alt text](wokwi-esp32.png)

*Sistema completo de monitoramento ambiental com todos os componentes interconectados*

## 🎥 Vídeo Demonstrativo

<div align="center">
https://img.shields.io/badge/%F0%9F%8E%AC_Assistir_V%C3%ADdeo_Completo-FF0000?style=for-the-badge&logo=youtube&logoColor=white

https://./assets/video-thumbnail.jpg

Clique na imagem acima para assistir à demonstração completa do projeto

</div>

## 🔧 **Instalação e Uso**

### 1. 📋 **Pré-requisitos**
```bash
# Bibliotecas necessárias
- WiFi.h
- PubSubClient.h  
- DHT.h (Adafruit)
```

### 2. 🛠 **Montagem no Wokwi**

```
ESP32 Pinout:
├── DHT22 (SDA)    → D4
├── LDR (AO)       → D34  
├── LED Verde      → D13 (+ resistor 220Ω)
├── LED Amarelo    → D12 (+ resistor 220Ω)
└── LED Vermelho   → D14 (+ resistor 220Ω)
```

### 3. ⚙ **Configuração**
```cpp
const char* SSID = "Wokwi-GUEST";
const char* BROKER_MQTT = "44.223.43.74";
const int BROKER_PORT = 1883;
const char* TOPICO_PUBLISH = "/TEF/device007/attrs";
```

### 4. 🚀 **Execução**
```bash
1. Abrir projeto no Wokwi
2. Compilar e executar simulação  
3. Monitorar Serial Monitor (115200 baud)
4. Testar diferentes condições ambientais

```

## 🎯 **Impacto e Benefícios**

### ✅ **Para Empresas**
- **+15-20%** de produtividade em ambientes ideais
- **-30%** em afastamentos por problemas de saúde
- **Dados** para otimização de espaços de trabalho

### ✅ **Para Trabalhadores**  
- **Prevenção** de problemas de saúde ocupacional
- **Consciência ambiental** do espaço de trabalho
- **Qualidade de vida** no home office

### ✅ **Para o Futuro do Trabalho**
- **Tecnologia humanizada** que cuida das pessoas
- **Ambientes adaptativos** e inteligentes
- **Trabalho remoto** mais saudável e sustentável

## 👥 **Equipe**

| Nome | RM | Contribuição |
|------|--------|--------------|
| **Júlio César Augusto Vieira** | 563366 | Arquitetura e Implementação |
| **João Batista Lima Neto** | 563426 | Documentação e Testes |

## 🔗 **Links Úteis**

- [**🌐 Simulação Wokwi**](https://wokwi.com/projects/seu-link-aqui)
- [**🎥 Vídeo Demonstrativo**](https://youtube.com/seu-link-aqui)  
- [**💻 Código Fonte**](https://github.com/seu-usuario/seu-repositorio)


---

<div align="center">

### 💼 **Global Solution 2025 - FIAP**

</div>