#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct graph{
    int node_number;
    int edge_number;
    int *community;
    int **matrix;
} graph;

int connected_edge(graph *g, int i, int j){
    return g->matrix[i][j];
}
double expected_value(graph *g, int i, int j){
    int k1 = 0, k2 = 0;
    for(int k = 0; k < g->node_number; k++){        
        k1 += g->matrix[i][k];
        k2 += g->matrix[j][k];
    }
    return ((double)k1 * (double)k2) / (2.0 * g->edge_number);
    
}
double modularity_network(graph *g){
    double Q = 0.0;
    
    for(int i = 0; i < g->node_number; i++){
        for(int j = 0; j < g->node_number; j++){
            if(g->community[i] == g->community[j]){
                Q += (connected_edge(g, i, j) - expected_value(g, i, j)) / (2.0 * g->edge_number);
            }
        }
    }
    return Q;
}
double modularity_singular(graph *g, int community_id){
    int commun_nodes[g->node_number];
    double Q = 0.0;
    int comm_size = 0;
    for(int i = 0; i < g->node_number; i++){
        if(g->community[i] == community_id){
            commun_nodes[comm_size] = i;
            comm_size++;
        }
}
    for(int i = 0; i < comm_size; i++){
        for(int j = i; j < comm_size; j++){   
            int ni = commun_nodes[i];
            int nj = commun_nodes[j];
            double delta = (connected_edge(g,ni,nj) -  expected_value(g,ni,nj)) / (2.0 * g->edge_number);
            if(i == j){
                Q += delta;
            } else {
                Q += 2 * delta;
            }
        }    
    }
    return Q;
}


void compute_degrees(graph *g, int *deg) {
    for (int i = 0; i < g->node_number; i++) {
        int d = 0;
        for (int j = 0; j < g->node_number; j++) d += g->matrix[i][j];
        deg[i] = d;
    }
}

void compute_comm_tot(graph *g, const int *deg, int *comm_tot) {
    memset(comm_tot, 0, sizeof(int) * g->node_number);
    for (int i = 0; i < g->node_number; i++) {
        comm_tot[g->community[i]] += deg[i];
    }
}


int ki_in_to_comm(graph *g, int i, int target_comm) {
    int s = 0;
    for (int j = 0; j < g->node_number; j++) {
        if (g->community[j] == target_comm && g->matrix[i][j]) s += g->matrix[i][j];
    }
    return s;
}

void phase1_v2(graph *g){
    int n = g->node_number;
    if (g->edge_number == 0) return;

    int *deg = (int *)malloc(sizeof(int) * n);
    int *comm_tot = (int *)malloc(sizeof(int) * n);
    compute_degrees(g, deg);
    compute_comm_tot(g, deg, comm_tot);

    int changed = 1;
    double two_m = 2.0 * (double)g->edge_number;

    while (changed) {
        changed = 0;

        for (int i = 0; i < n; i++) {
            int current = g->community[i];
            int ki = deg[i];
            if (ki == 0) continue;

            
            int ki_in_old = ki_in_to_comm(g, i, current) - g->matrix[i][i];

            double best_gain = 0.0;
            int best_comm = current;

            
            for (int j = 0; j < n; j++) {
                if (!g->matrix[i][j]) continue;
                int target = g->community[j];
                if (target == current) continue;
                int used = 0;
                for(int k = 0; k < j; k++){
                    if(g->community[k] == target && g->matrix[i][k]){
                        used = 1;
                        break;
                    }
                }
                if (used) continue;

                int ki_in_target = ki_in_to_comm(g, i, target);

                int tot_old = comm_tot[current] - ki;
                int tot_new = comm_tot[target];

                // islem q : (ki_in_target - ki_in_old)/2m + ki*(tot_old - tot_new)/(2m)^2
                double delta_q =
                    ((double)(ki_in_target - ki_in_old) / two_m) +
                    ((double)ki * (double)(tot_old - tot_new)) / (two_m * two_m);

                if (delta_q > best_gain) {
                    best_gain = delta_q;
                    best_comm = target;
                }
            }
            g->community[i] = best_comm;
            comm_tot[current] -= ki;
            comm_tot[best_comm] += ki;
            
            
            if (best_comm != current) {
                changed = 1;
            }
        }
    }

    free(deg);
    free(comm_tot);
}


void phase2(graph *g){
    int used[g->node_number];
    memset(used, 0, sizeof(used)); 
    int community_number = 0;

    for(int i = 0; i < g->node_number; i++){
        if(!used[g->community[i]]){
            used[g->community[i]] = 1;
            community_number++;
        }
    }
    
    int *communities = (int *)malloc(community_number * sizeof(int));
    memset(used, 0, sizeof(used));
    int index = 0;
    for(int i = 0; i < g->node_number; i++){
        if(!used[g->community[i]]){
            used[g->community[i]] = 1;
            communities[index] = g->community[i];
            index++;
        }
    }
    
    graph g2;
    g2.edge_number = 0;
    g2.node_number = community_number;
    g2.matrix = (int**)malloc(g2.node_number * sizeof(int *));
    g2.community = (int *)malloc(g2.node_number * sizeof(int));
    
    for(int i = 0; i < g2.node_number; i++){
        g2.community[i] = i;
        g2.matrix[i] = (int *)calloc(g2.node_number, sizeof(int));
    }
    
    for(int i = 0; i < g->node_number; i++){
        for(int j = 0; j < g->node_number; j++){
            if(g->matrix[i][j]){
                int index_i = -1, index_j = -1;
                for(int k = 0; k < community_number; k++){
                    if(communities[k] == g->community[i]) index_i = k;
                    if(communities[k] == g->community[j]) index_j = k;
                }
                g2.matrix[index_i][index_j] += g->matrix[i][j];
            }
        }
    }
    for(int i = 0; i < g2.node_number; i++){
        for(int j = i; j < g2.node_number; j++){
            if(i == j){
                g2.edge_number += g2.matrix[i][j] / 2;
            } else {
                g2.edge_number += g2.matrix[i][j];
            }
        }
    }
    
    free(communities);
    for(int i = 0; i < g->node_number; i++){
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g->community);
    *g = g2;
}


void fast_greedy(graph *g){
    int n = g->node_number;
    if (g->edge_number == 0) return;

    int *deg = (int *)malloc(sizeof(int) * n);
    int *community_degrees = (int *)malloc(sizeof(int) * n);
    compute_degrees(g, deg);
    compute_comm_tot(g, deg, community_degrees);
    double m = (double)g->edge_number;
    while (1) {
        
        int best_a = -1, best_b = -1;
        double max_delta_q = -1.0;  
        
        for(int i = 0; i < n; i++){
            if(community_degrees[i] == 0) continue;
        
            for(int j = i + 1; j < n; j++){
                if(community_degrees[j] == 0) continue;

                int edges_between = 0;

                for(int k = 0; k < n; k++){
                    if(g->community[k] == i){
                        for(int l = 0; l < n; l++){
                            if(g->community[l] == j){
                                edges_between += g->matrix[k][l];
                            }
                        }
                    }
                }
                double delta_q = 
                    ((double)edges_between / m) - 
                    ((double)(community_degrees[i] * community_degrees[j]) / (2.0 * m * m));
                    
                if(delta_q > max_delta_q){
                    max_delta_q = delta_q;
                    best_a = i;
                    best_b = j;
                }
            }
        }
        
        if(best_a == -1 || max_delta_q < -1e-10){
            break;
        }
        
        for(int i = 0; i < n; i++){
            if(g->community[i] == best_b){
                g->community[i] = best_a;
            }
        }
        community_degrees[best_a] += community_degrees[best_b];
        community_degrees[best_b] = 0;
    }
    free(deg);
    free(community_degrees);
}

void my_algorithm(graph *g){

    int n = g->node_number;
    g->community = malloc(n * sizeof(int));

    for(int i = 0; i < n; i++)
        g->community[i] = i;

    int changed = 1;

    while(changed){
        changed = 0;

        for(int i = 0; i < n; i++){

            int best_comm = g->community[i];
            int best_score = -1;

            for(int j = 0; j < n; j++){
                if(g->matrix[i][j] == 1 && i != j){
                    int common = 0;
                    for(int k = 0; k < n; k++){
                        if(g->matrix[i][k] == 1 && g->matrix[j][k] == 1)
                            common++;
                    }
                    if(common > best_score){
                        best_score = common;
                        best_comm = g->community[j];
                    }
                }
            }

            if(best_comm != g->community[i]){
                g->community[i] = best_comm;
                changed = 1;
            }
        }
    }
}


void my_algorithm_2(graph *g){
    int *most_neigbour_comm = malloc(g->node_number * sizeof(int));
    int changed = 1;
    while(changed){
        changed = 0; 
        for(int i = 0; i< g->node_number; i++){
            double Q = modularity_network(g);
            for(int k = 0; k < g->node_number; k++){
                most_neigbour_comm[k] = 0;
            }
            for(int j = 0; j < g->node_number; j++){
                if(g->matrix[i][j] == 1 && g->community[i] != g->community[j]){
                    most_neigbour_comm[g->community[j]]++;
                }    
            }  
            int max_count = 0;
            int best_comm = g->community[i];
            for(int c = 0; c < g->node_number; c++){
                if(most_neigbour_comm[c] > max_count){
                    max_count = most_neigbour_comm[c];
                    best_comm = c;
                }
            }
            int prev_comm = g->community[i];
            g->community[i] = best_comm;
            double new_Q = modularity_network(g);
            if(new_Q > Q){
                changed = 1;
            } else {
                g->community[i] = prev_comm; 
            }
        }
    }
    free(most_neigbour_comm);
}

int main() {
    FILE *data = fopen("data.txt", "r");
    if (data == NULL) {
        perror("Hata");
        return EXIT_FAILURE;    
    }
    
    graph g;
    graph g2;
    graph g3; 
    graph g4; 
    g.node_number = 0;
    g.edge_number = 0;
    g2.node_number = 0;
    g2.edge_number = 0;
    g3.node_number = 0;
    g3.edge_number = 0;
    g4.node_number = 0;
    g4.edge_number = 0;
    int max = 0;
    int u, v;
    char line[1000];
    
    while(fgets(line, sizeof(line), data)) {
        char *token = line;
        while(*token) {
            if(sscanf(token, "[%d %d]", &u, &v) == 2) {
                if(u > max) max = u;
                if(v > max) max = v;
                g.edge_number++;
                token = strchr(token, ']');
                if(token == NULL) break;
                token++;
            } else {
                token++;
            }
        }
    }

    g.node_number = max;
    g2.node_number = max;
    g3.node_number = max;
    g4.node_number = max;
    g.matrix = (int **)malloc(g.node_number * sizeof(int *));
    for(int i = 0; i < g.node_number; i++) {
        g.matrix[i] = (int *)calloc(g.node_number, sizeof(int));
    }
    g2.matrix = (int **)malloc(g2.node_number * sizeof(int *));
    for(int i = 0; i < g2.node_number; i++) {
        g2.matrix[i] = (int *)calloc(g2.node_number, sizeof(int));
    }
    g3.matrix = (int **)malloc(g3.node_number * sizeof(int *));
    for(int i = 0; i < g3.node_number; i++) {
        g3.matrix[i] = (int *)calloc(g3.node_number, sizeof(int));
    }
    g4.matrix = (int **)malloc(g4.node_number * sizeof(int *)); 
    for(int i = 0; i < g4.node_number; i++) {
        g4.matrix[i] = (int *)calloc(g4.node_number, sizeof(int));
    }

    
    rewind(data);
    int edge_count = 0;
    while(fgets(line, sizeof(line), data)){
        char *token = line;
        while(*token) {
            if(sscanf(token, "[%d %d]", &u, &v) == 2) {
                if(!g.matrix[u-1][v-1]){  
                    g.matrix[u-1][v-1] = 1;
                    g.matrix[v-1][u-1] = 1;
                    g2.matrix[u-1][v-1] = 1;
                    g2.matrix[v-1][u-1] = 1;
                    g3.matrix[u-1][v-1] = 1;
                    g3.matrix[v-1][u-1] = 1;
                    g4.matrix[u-1][v-1] = 1; 
                    g4.matrix[v-1][u-1] = 1;
                    edge_count++;
                }
                token = strchr(token, ']');
                if(token == NULL) break;
                token++;
            } else {
                token++;
            }
        }
    }
    fclose(data);
    
    g.edge_number = edge_count;  
    g2.edge_number = edge_count;
    g3.edge_number = edge_count;
    g4.edge_number = edge_count;

    g.community = (int *)malloc(g.node_number * sizeof(int));
    for(int i = 0; i < g.node_number; i++){
        g.community[i] = i;
    }
    g2.community = (int *)malloc(g2.node_number * sizeof(int));
    for(int i = 0; i < g2.node_number; i++){
        g2.community[i] = i;
    }
    g3.community = (int *)malloc(g3.node_number * sizeof(int));
    for(int i = 0; i < g3.node_number; i++){
        g3.community[i] = i;
    }
    g4.community = (int *)malloc(g4.node_number * sizeof(int));
    for(int i = 0; i < g4.node_number; i++){
        g4.community[i] = i;
    }
    
    
    
    printf("Node : %d, Kenar : %d\n", g.node_number, g.edge_number);
    
    double prev_Q = -1.0;
    double Q = 0.0;
    int deneme = 0;
    int max_den = 100;   
    

    while(deneme < max_den) {
        phase1_v2(&g);
        int communities_found[g.node_number];
        int comm_count = 0;
        for(int i = 0; i < g.node_number; i++){
            int found = 0;
            for(int j = 0; j < comm_count; j++){
                if(communities_found[j] == g.community[i]){
                    found = 1;
                    break;
                }
            }
            if(!found){
                communities_found[comm_count] = g.community[i];
                comm_count++;
            }
        }
        Q = modularity_network(&g);
        deneme++;
        printf("deneme %d: Q : %lf, Community Sayisi= %d\n", deneme, Q, comm_count);
        
        if(Q - prev_Q < 1e-6) break;
        prev_Q = Q;
        
        if(g.node_number == 1) break;
        phase2(&g);
    }

    printf("Louvain node-community:\n");
    for(int c = 0; c < g.node_number; c++) {
        int found = 0;
        for(int i = 0; i < g.node_number; i++) {
            if(g.community[i] == c) {
                if(!found) {
                    printf("Community %d :", c);
                    found = 1;
                }
                printf(" %d", i+1);
            }
        }
        if(found) printf("\n");
    }
    free(g.community);
    for(int i = 0; i < g.node_number; i++) {
        free(g.matrix[i]);
    }
    free(g.matrix);

    printf("\n Fast Greedy \n");
    
    fast_greedy(&g2);
    
    int communities_found[g2.node_number];
    int comm_count = 0;
    for(int i = 0; i < g2.node_number; i++){
        int found = 0;
        for(int j = 0; j < comm_count; j++){
            if(communities_found[j] == g2.community[i]){
                found = 1;
                break;
            }
        }
        if(!found){
            communities_found[comm_count] = g2.community[i];
            comm_count++;
        }
    }
    Q = modularity_network(&g2);
    printf("Q : %lf, commun= %d\n", Q, comm_count);

    printf("Fast Greedy node-community:\n");
    for(int c = 0; c < g2.node_number; c++) {
        int found = 0;
        for(int i = 0; i < g2.node_number; i++) {
            if(g2.community[i] == c) {
                if(!found) {
                    printf("Community %d :", c);
                    found = 1;
                }
                printf(" %d", i+1);
            }
        }
        if(found) printf("\n");
    }
    free(g2.community);
    for(int i = 0; i < g2.node_number; i++) {
        free(g2.matrix[i]);
    }
    free(g2.matrix);

    
    printf("\n My Algorithm \n");
    my_algorithm(&g3);

    int communities_found_my[g3.node_number];
    int comm_count_my = 0;
    for(int i = 0; i < g3.node_number; i++){
        int found = 0;
        for(int j = 0; j < comm_count_my; j++){
            if(communities_found_my[j] == g3.community[i]){
                found = 1;
                break;
            }
        }
        if(!found){
            communities_found_my[comm_count_my] = g3.community[i];
            comm_count_my++;
        }
    }
    Q = modularity_network(&g3);
    printf("Q : %lf, commun= %d\n", Q, comm_count_my);

    printf("My Algorithm node-community:\n");
    for(int c = 0; c < g3.node_number; c++) {
        int found = 0;
        for(int i = 0; i < g3.node_number; i++) {
            if(g3.community[i] == c) {
                if(!found) {
                    printf("Community %d :", c);
                    found = 1;
                }
                printf(" %d", i+1);
            }
        }
        if(found) printf("\n");
    }
    free(g3.community);
    for(int i = 0; i < g3.node_number; i++) {
        free(g3.matrix[i]);
    }
    free(g3.matrix);

    
    printf("\n My Algorithm 2 \n");
    my_algorithm_2(&g4);

    int communities_found_my2[g4.node_number];
    int comm_count_my2 = 0;
    for(int i = 0; i < g4.node_number; i++){
        int found = 0;
        for(int j = 0; j < comm_count_my2; j++){
            if(communities_found_my2[j] == g4.community[i]){
                found = 1;
                break;
            }
        }
        if(!found){
            communities_found_my2[comm_count_my2] = g4.community[i];
            comm_count_my2++;
        }
    }
    Q = modularity_network(&g4);
    printf("Q : %lf, commun= %d\n", Q, comm_count_my2);

    printf("My Algorithm 2 node-community:\n");
    for(int c = 0; c < g4.node_number; c++) {
        int found = 0;
        for(int i = 0; i < g4.node_number; i++) {
            if(g4.community[i] == c) {
                if(!found) {
                    printf("Community %d :", c);
                    found = 1;
                }
                printf(" %d", i+1);
            }
        }
        if(found) printf("\n");
    }
    free(g4.community);
    for(int i = 0; i < g4.node_number; i++) {
        free(g4.matrix[i]);
    }
    free(g4.matrix);
    
    return 0;
}