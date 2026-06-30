#ifndef CONVERTER_H
#define CONVERTER_H

/**
 * Recebe um arquivo csv com os dados das salas de aula e constrói um JSON com os dados
 * Retorno: 1 se sucesso, 0 se erro
 */
int classrooms_to_json(const char *csv);

/**
 * Recebe um arquivo .csv com os dados dos prédios e outro com as coordenadas de contorno de cada prédio
 * Constrói um JSON com os dados combinados dos dois .csv
 * Retorno: 1 se sucesso, 0 se erro
 */
int buildings_to_json(const char *csv, const char *csv_poly);

/**
 * Recebe um arquivo .csv com os dados dos campi e outro com as coordenadas de contorno de cada campus
 * Constrói um JSON com os dados combinados dos dois arquivos .csv
 * Retorno: 1 se sucesso, 0 se erro
 */
int campuses_to_json(const char *csv, const char *csv_poly);

#endif