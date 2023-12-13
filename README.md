**Descrição:**

Este código implementa um sistema de Text-to-Speech (TTS) para ESP32, utilizando o FreeRTOS e a biblioteca Flite. 
O código utiliza o periférico I2S para a saída de áudio e inclui uma tarefa dedicada para a síntese de voz. 
A voz é sintetizada a partir de texto enviado para uma fila, e o resultado é reproduzido por meio do I2S.

**Recursos Principais:**

•	Síntese de voz utilizando a biblioteca Flite.

•	Utilização do periférico I2S para a saída de áudio.

•	Implementação de uma fila para a comunicação entre a aplicação principal e a tarefa de síntese de voz.

•	Uso do FreeRTOS para gerenciamento de tarefas e fila.

**Observação:**

Certifique-se de ajustar as configurações do I2S (pino BCK, pino WS, pino de dados, etc.) no arquivo de configuração do projeto (sdkconfig.h ou similar) para refletir corretamente a configuração do seu hardware específico.

Desenvolvido na IDF v5.1.2.
