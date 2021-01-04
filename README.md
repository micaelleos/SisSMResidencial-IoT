# SisSMResidencial-IoT

Sistemas baseados em IOT tornaram-se ferramentas poderosas no cotidiano humano nos últimos anos. Este repositório possui os códigos de um sistema de segurança e monitoramento residencial  desenvolvido com o uso do microcontrolador DSPIC30F4011 como unidade central de processamento, módulo WiFi WSP8266 para conectividade com a internet a partir do protocolo MQTT, módulo RTC para informação sobre hora durante atuação de sensores, e a plataforma Cayenne para interação com o usuário.

## Arquitetura de Comunicação

![image](https://user-images.githubusercontent.com/61849864/103585448-d1c5fc00-4ec1-11eb-84c3-26b5fdb6c43a.png)
 
## Funcionamento

Sistema de segurança residencial permite o usuário configurar um período do dia em que os acessos da casa estão disponíveis para entrada, mas que essas informações sejam salvas no histórico. Quando existe alguma entrada na casa fora do período do dia permitido, uma notificação chega ao usuário avisando que existe um intruso, e um sistema de alarme soa na casa. O usuário tem a possibilidade, caso o sistema de alarme tenha disparado, de desligar o alarme com uma senha, que pode ser digitada tanto no celular, computador e pessoalmente na casa.

O que o programa precisa fazer:
-	Configuração de hora: será feito com dois botões e o LCD 
-	Todo acesso a casa precisa ser enviado a plataforma Cayenne
-	Verificação se acesso foi feito no período permitido
-	Soar alarme com buzzer e luzes da casa piscando
-	Caso alarme acionado, ter a possibilidade de entrada de senha pelos botões + LCD, computador e celular.
