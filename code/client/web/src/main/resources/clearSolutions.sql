UPDATE alunos_demanda SET ald_atendido = b'0';
DELETE FROM alunos_demanda_atendimentos_operacional;
DELETE FROM alunos_demanda_atendimentos_tatico;
DELETE FROM atendimento_tatico;
DELETE FROM atendimento_operacional_motivos_uso;
DELETE FROM atendimento_operacional;
