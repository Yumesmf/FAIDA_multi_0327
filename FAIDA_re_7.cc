// multi-thread
#include "faida.h"

// read csv, get the size of columns and records
pair<CSVDATA, CSVDATA> get_record_col_count(const char *path, CSVDATA csvdata, CSVDATA csvdata_hash)
{
    int record_count = 0;
    int col_count = 0;
    FILE *fp = fopen(path, "r");

    while (fgets(linebuf, LINE_BUF_SIZE, fp))
    {
        col_count = 0;

        word = strtok_r(linebuf, ",", &tmp);

        while (word != NULL)
        {
            strtol(word, NULL, 0);
            col_count++;
            word = strtok_r(NULL, ",", &tmp);
        }

        record_count++;
    }

    csvdata.record_count = record_count;
    csvdata.col_count = col_count;
    csvdata_hash.record_count = record_count;
    csvdata_hash.col_count = col_count;

    /*
    csvdata.data.resize(record_count);
    for (int i = 0; i < record_count; i++)
    {
        csvdata.data[i].resize(col_count);
    }

    csvdata_hash.data.resize(record_count);
    for (int i = 0; i < record_count; i++)
    {
        csvdata_hash.data[i].resize(col_count);
    }
    */
    fclose(fp);

    csvdata = insert_structure(csvdata, csvdata_hash, csvdata.record_count, csvdata.col_count, fp, path).first;
    csvdata_hash = insert_structure(csvdata, csvdata_hash, csvdata.record_count, csvdata.col_count, fp, path).second;
    return make_pair(csvdata, csvdata_hash);
}

// insert csv data into struct
pair<CSVDATA, CSVDATA> insert_structure(CSVDATA csvdata, CSVDATA csvdata_hash, int record_count, int col_count, FILE *fp, const char *path)
{
    string word_hash;
    fp = fopen(path, "r");
    auto rec_p = cvsdata.data.head();
    auto h_rec_p = cvsdata_hash.data.head();
    while (fgets(linebuf, LINE_BUF_SIZE, fp)) {
	word = strtok_r(linebuf, ",", &tmp);
	if (word == NULL) {
	    fclose(fp);
	    return make_pair(csvdata, csvdata_hash);
	}
	auto col_p = recp.head();
	auto h_col_p = recp.head();
	hash<string> hash_obj;
	while (true) {
	    // normal
	    colp.push_back(word);

	    // hash
	    stringstream hash_data; hash_data << hash_obj(word);
	    word_hash = hash_data.str();
	    h_col_p.push_back(word_hash);	    

	    word = strtok_r(NULL, ",", &tmp);
	}
    }
}

void print_csv(CSVDATA csvdata)
{
    for (auto rec = cvsdata.data.begin(); rec != cvsdata.data.end(); rec++) {
	for (auto col = rec->begin(); col != rec->end(); col++) {
            cout << *col << endl;
        }
    }
}

// hll, inverted index, generate ind
pair<vector<string>, vector<string> > HLL(CSVDATA csvData_1, CSVDATA csvData_1_hash, CSVDATA csvData_2, CSVDATA csvData_2_hash)
{
    // HLL for csv1 and csv2, hashvalue->binary->first four bits
    int i = 0;
    //while (i < csvData_1_hash.record_count)
    //while (i < csvData_1_hash.size()) {
    for (auto h_rec_p = csvData_1_hash.data.begin(); h_rec_p != csvData_1_hash.data.end() h_rec_p++) {
        string hll = "";
	for (auto h_col_p = h_rec_p->begin(); h_col_p != h_rec_p->end() h_col_p++) {
            hll = hll + StrToBitStr(*h_col_p);
        }
        hash_hll_1.push_back(hll);
    }

    int m = 0;
    for (auto h_rec_p = csvData_2_hash.data.begin(); h_rec_p != csvData_2_hash.data.end() h_rec_p++) {
        string hll = "";
	for (auto h_col_p = h_rec_p->begin(); h_col_p != h_rec_p->end() h_col_p++) {
            hll = hll + StrToBitStr(*h_col_p);
        }
        hash_hll_2.push_back(hll);
    }

    return make_pair(hash_hll_1, hash_hll_2);
}

// get binary HLL
string StrToBitStr(string str)
{
    str = str.substr(0, 19);
    long binary;
    binary = atol(str.c_str());

    string r;
    while (binary != 0)
    {
        r += (binary % 2 == 0 ? "0" : "1");
        binary /= 2;
    }
    reverse(begin(r), end(r));
    // cout << r << endl;
    string s = r.substr(0, 4);
    return s;
}

void *IND_check(void *arg)
{
    // compare HLL binary string by tuple, check each value (= inverted index)
    int start = (long)arg * 40000;
    cout << start << endl;
    for (int i = start; i < start + 40000; i++)
    {
        for (int j = 0; j < hash_hll_2.size(); j++)
        {
            if (hash_hll_1[i] == hash_hll_2[j])
            {
                if (csvData_1_hash.data[i] == csvData_2_hash.data[j])
                {
                    record.push_back(i);
                }
            }
        }
    }
    // cout << 1 << endl;
    // for (int i = 0; i < record.size(); i++)
    // {
    //     cout << record[i] << endl;
    // }
    sort(record.begin(), record.end());
    record.erase(unique(record.begin(), record.end()), record.end());

    return NULL;
    // result output
}

// void output(vector<int> record, CSVDATA csvData_1)
// {
//     sort(record.begin(), record.end());
//     record.erase(unique(record.begin(), record.end()), record.end());
//     ofstream outFile;
//     outFile.open("data555.csv", ios::out);
//     for (int i = 0; i < record.size(); i++)
//     {
//         for (int j = 0; j < csvData_1.col_count; j++)
//         {
//             outFile << csvData_1.data[record[i]][j] << endl;
//         }
//     }
//     outFile.close();
// }
// void thread_hll()
// {

// }

int main(void)
{

    string filepath1_1 = "customer.csv";
    string filepath2_1 = "supplier.csv";
    const char *filepath1 = filepath1_1.c_str();
    const char *filepath2 = filepath2_1.c_str();
    csvData_1 = get_record_col_count(filepath1, csvData_1, csvData_1_hash).first;
    csvData_1_hash = get_record_col_count(filepath1, csvData_1, csvData_1_hash).second;
    csvData_2 = get_record_col_count(filepath2, csvData_2, csvData_2_hash).first;
    csvData_2_hash = get_record_col_count(filepath2, csvData_2, csvData_2_hash).second;
    // print_csv(csvData_1);
    // print_csv(csvData_1_hash);
    // print_csv(csvData_2);
    // print_csv(csvData_2_hash);
    hash_hll_1 = HLL(csvData_1, csvData_1_hash, csvData_2, csvData_2_hash).first;
    hash_hll_2 = HLL(csvData_1, csvData_1_hash, csvData_2, csvData_2_hash).second;
    // IND_check(hash_hll_1, hash_hll_2, csvData_1_hash, csvData_2_hash, csvData_1);
    // thread_hll();
    clock_t begin = clock();
    pthread_t threads[NUM_THREADS];

    for (int m = 0; m < NUM_THREADS; m++){
        int rc;
        rc = pthread_create(&threads[m], NULL, IND_check, (void *)m);
        if (rc) {perror(""); exit(1);}
    }
    for (int m = 0; m < NUM_THREADS; m++) {
        pthread_join(threads[m], NULL);
    }
    ofstream outFile;
    outFile.open("data555.csv", ios::out);
    for (int i = 0; i < record.size(); i++) {
        for (int j = 0; j < csvData_1.col_count; j++) {
            outFile << csvData_1.data[record[i]][j] << endl;
        }
    }
    outFile.close();
    clock_t end = clock();
    cout << "Running time: " << (double)(end - begin) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    // merge(record1.begin(), record1.end(), record2.begin(), record2.end(), record4.begin());
    // merge(record4.begin(), record4.end(), record3.begin(), record3.end(), record5.begin());
    // output(record, csvData_1);
}

/*
目前最佳版本
可读csv行列数
*/
