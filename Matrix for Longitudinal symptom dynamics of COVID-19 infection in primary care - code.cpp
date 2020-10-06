
	if (CORONA_ARTICLE_QUEST_MACCABI == 1) {
		

		string symp_dir = "C:\\corona_segal\\";
		string symp_fname = symp_dir + "corona_quest";
		string tests = symp_dir + "tests";
		string diag_fname = symp_dir + "diagnosis";
		string pop_fname_merge = symp_dir + "pop";
		string hosp_fname = symp_dir + "hosp";
		string isolation_fname = symp_dir + "Isolation";






		map<int, int> segal_info;
		if (1 == 1) {
			int count = 0;
			string fname = "segal_info_indication.txt";
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					int code = stoi(fields[1]);

					int ind = 0;
					if (code > 0) {
						segal_info[id] = 1;
					}
				}
			}
			fprintf(stderr, "segal_info %i\n", (int)segal_info.size());
		}


		//read-isolation
		map<int, int> map_id_isolation;
		if (1 == 1) {
			int count = 0;
			string fname = isolation_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					int isol_code = stoi(fields[1]);

					//fprintf(stderr, "%i %i \n", id, isol_code);
					//getchar();

					if (isol_code == 1916 || isol_code == 1917 || isol_code == 1919) {
						map_id_isolation[id] = 1;
					}
				}
			}
		}
		fprintf(stderr, "map_id_isolation size %i \n", (int)map_id_isolation.size());
		//getchar();


		//read-pop_file 
		map<int, int> map_id_recover;
		if (1 == 1) {
			int count = 0;
			string fname = pop_fname_merge;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					int age = stoi(fields[4]);
					int gender = stoi(fields[5]);


					if (fields[2] == "1") {
						map_id_recover[id] = convert_date(fields[3]);
						//fprintf(stderr, "%i\n", map_id_recover[id]);
						//getchar();
					}
				}
			}
		}
		fprintf(stderr, "recover size %i \n", (int)map_id_recover.size());


		//read-pop_file 
		map<int, int> map_id_doctor;
		if (1 == 1) {
			int count = 0;
			string fname = "C:\\corona_segal\\doctors1.txt";;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					map_id_doctor[id] = 1;
				}
			}
		}
		fprintf(stderr, "recover size %i \n", (int)map_id_recover.size());


		//hosp
		map<int, int> map_id_hosp;
		if (1 == 1) {
			int count = 0;
			string fname = hosp_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					map_id_hosp[id] = 1;
					//fprintf(stderr, "%i\n", id);
					//getchar();
				}
			}
		}


		//read tests
		map<int, int> test_neg_first;
		map<int, int> test_pos_first;
		map<int, int>  test_any;
		if (1 == 1) {
			map<string, int> tests_names_map;
			int count = 0;
			string fname = tests;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int jj = 0; jj < fields.size(); jj++)
							tests_names_map[fields[jj]] = jj;
						continue;
					}

					split(fields, curr_line, boost::is_any_of("\t"));
					//int id = stoi(fields[0]);
					int id = stoi(fields[tests_names_map["RANDOM_ID"]]);
					string SAMPLE_RESULT_DATE = fields[tests_names_map["SAMPLE_RESULT_DATE"]];
					string execution_date = fields[tests_names_map["SAMPLE_EXECUTION_DATE"]];
					int TEST_RESULT_CD = stoi(fields[tests_names_map["TEST_RESULT_CD"]]);
					string final_date;
					if (execution_date == "1800-01-01 00:00:00.000" && SAMPLE_RESULT_DATE == "1/1/2020") continue;
					if (execution_date == "1800-01-01 00:00:00.000" && convert_date(SAMPLE_RESULT_DATE) == 20200101) continue;

					int new_date;
					if (execution_date != "1800-01-01 00:00:00.000" && convert_date(execution_date) <= datefilter) {
						final_date = execution_date;
						new_date = convert_date(final_date);
					}
					else {
						final_date = SAMPLE_RESULT_DATE;
						new_date = convert_date(final_date);
					}

					//fprintf(stderr, "new_date %i\n", new_date);
					//getchar();

					if (TEST_RESULT_CD == 1) {
						test_any[id] = 1;
						if (test_neg_first.find(id) == test_neg_first.end())
							test_neg_first[id] = new_date;
						else if (new_date < test_neg_first[id]) test_neg_first[id] = new_date;
					}

					if (TEST_RESULT_CD == 2) {
						test_any[id] = 1;
						if (test_pos_first.find(id) == test_pos_first.end())
							test_pos_first[id] = new_date;
						else if (new_date < test_pos_first[id]) test_pos_first[id] = new_date;
					}
				}
			}
		}


		vector<int> vec_neg_dates;
		for (auto it = test_neg_first.begin(); it != test_neg_first.end(); it++) {
			vec_neg_dates.push_back(it->second);
		}
		int n_vec_neg_dates = (int)vec_neg_dates.size();

		//read_diagnosis
		map<int, int> id_diagnosys_corona_date;
		if (1 == 2) {
			map<string, int> map_diagnosis_names;
			map<int, int>id_diagnosys_corona;

			int count = 0;
			string fname = diag_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int jj = 0; jj < fields.size(); jj++)
							map_diagnosis_names[fields[jj]] = jj;
						continue;
					}
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[map_diagnosis_names["RANDOM_ID"]]);
					int DIAGNOSIS_TYPE_CODE = stoi(fields[map_diagnosis_names["DIAGNOSIS_TYPE_CODE"]]);
					string VISIT_DATE = fields[map_diagnosis_names["VISIT_DATE"]];
					int VISIT_DATE_ki = convert_date(VISIT_DATE);

					//fprintf(stderr, "%i\n", VISIT_DATE_ki);
					//getchar();

					if (DIAGNOSIS_TYPE_CODE == 3 && id_diagnosys_corona.find(id) == id_diagnosys_corona.end()) {
						id_diagnosys_corona[id] = 1;
						id_diagnosys_corona_date[id] = VISIT_DATE_ki;
					}
					else if (DIAGNOSIS_TYPE_CODE == 3 && id_diagnosys_corona.find(id) != id_diagnosys_corona.end()) {
						if (VISIT_DATE_ki < id_diagnosys_corona_date[id])  id_diagnosys_corona_date[id] = VISIT_DATE_ki;
					}
				}
			}
		}



		map<int, int> corona_pos_final;
		for (auto it = test_pos_first.begin(); it != test_pos_first.end(); it++) {
			int id = it->first;
			int pos_date = it->second;
			corona_pos_final[id] = pos_date;
			if (id_diagnosys_corona_date.find(id) != id_diagnosys_corona_date.end()) {
				if (id_diagnosys_corona_date[id] < pos_date) corona_pos_final[id] = id_diagnosys_corona_date[id];
			}
		}
		fprintf(stderr, "corona_pos_final : %i \n", (int)corona_pos_final.size());
		//getchar();

		int count_isolation = 0;
		int count_all = 0;
		for (auto it = id_diagnosys_corona_date.begin(); it != id_diagnosys_corona_date.end(); it++) {
			int id = it->first;
			int diag_date = it->second;
			//corona_pos_final[id] = diag_date;
			if (corona_pos_final.find(id) == corona_pos_final.end()) {
				count_all++;
				if (map_id_recover.find(id) != map_id_recover.end()) {
					corona_pos_final[id] = diag_date;
				}

				if (map_id_hosp.find(id) != map_id_hosp.end()) {
					corona_pos_final[id] = diag_date;
				}

				if (map_id_isolation.find(id) != map_id_isolation.end()) {
					corona_pos_final[id] = diag_date;
					count_isolation++;
				}
			}

			if (test_any.find(id) == test_any.end()) test_any[id] = 1;
		}
		fprintf(stderr, "count_isolation : %i\n", count_isolation);
		fprintf(stderr, "count_all : %i\n", count_all);
		fprintf(stderr, "corona_pos_final : %i \n", (int) corona_pos_final.size());
		//getchar();



		//for (int mm = 0; mm < 6; mm++) {
		for (int mm = 0; mm < 1; mm++) {
		//for (int mm = 2; mm < 3; mm++) {

			map<int, int> map_qa_chul;


			//read_symptoms
			map<int, string> map_symp_names;
			map<string, vector<string>> map_symp_matrix;
			map<int, map<int, vector<string>>> map_id_date_all_symp;
			map<int, int> id_tzevet;
			map<int, int> id_age;
			map<int, int> id_gender;
			map<int, int> id_corona_ind;
			map<int, int> id_current_smoker;
			map<int, int> id_never_smoker;
			map<int, int> id_ex_smoker;
			map<int, int> id_num_of_quest;
			map<int, int> id_num_of_quest_before_test;
			map<int, int> id_num_of_quest_after_or_equal_test;

			map<int, int> id_bidud_from_chul;
			map<int, int> id_bidud_from_corona_patient;
			map<int, int> id_bidud_from_tasminim;
			map<int, int> id_bidud_from_personal;
			map<int, int> id_bidud_other;

			//map<int, int> id_bidud_from_chul_pre;
			//map<int, int> id_bidud_from_corona_patient_pre;
			//map<int, int> id_bidud_from_tasminim_pre;
			//map<int, int> id_bidud_from_personal_pre;
			//map<int, int> id_bidud_other_pre;

			//read questionnaire sympton file
			if (1 == 1) {



				//string fname_out = symp_dir + "symp_with_corona.txt";
				//FILE *fout = fopen(fname_out.c_str(), "w");
				map<string, int> map_qa;
				map<int, int> map_qa1;
				map<int, int> map_qa_milui_date_n;

				int count_milui_in = 0;
				int count_milui_out = 0;
				int count_fix_chom = 0;
				map<string, int> map_chom;
				map<int, int> article_milui_dates;

				map<int, int> article_age_all;
				map<int,map<int, int>> article_all_before;
				map<int, map<int, int>> article_all_after;

				int count = 0;
				string fname = symp_fname;
				fprintf(stderr, "======read %s \n", fname.c_str());
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						count++;
						if (count % 100000 == 0) fprintf(stderr, "%i\n", count);
						if (count == 1) {
							split(fields, curr_line, boost::is_any_of(","));
							for (int ii = 0; ii < fields.size(); ii++)
								map_symp_names[ii] = fields[ii];
							continue;
						}

						split(fields, curr_line, boost::is_any_of("\t"));
						//fprintf(stderr, "i am here 1 \n" );

						//if (count > 1000000) {
							//fprintf(stderr, "%s\n", fields[2].c_str());
						//}

						int id = stoi(fields[2]);

						//if (map_id_doctor.find(id) == map_id_doctor.end()) continue;

						map_qa1[(int)fields.size()]++;
						
						//for (int mm = 10; mm < 20; mm++) {
						//}

						//fprintf(stderr, "%d", fields[11].substr(0, 1));
						//fprintf(stderr, "%d", fields[11].substr(1, 1));



						//getchar();

						


						//fprintf(stderr, "i am here 12 \n");

						string milui_date = fields[1];
						

						//if ((int)fields.size() != 59) {
							//fprintf(stderr, "%s\t%i\n", fields[0].c_str(), (int)fields.size());
							//getchar();
						//}

						

						//fprintf(stderr, "%s\n", fields[57].c_str());
						int tzevet = 0;//	if (fields[57]!="") stoib(fields[57]);

						//continue;
						

						//fprintf(stderr, "i am here 14 \n");
						int age = stoi(fields[3]);

						
						

						//fprintf(stderr, "i am here 15 \n");
						int gender = stoi(fields[4]);


						//fprintf(stderr, "i am here 16 \n");
						string bidud_date = fields[9];

					
						//fprintf(stderr, "i am here 2 \n");

						//if (gender == 0) continue;

						//vector<string> milui_date_vec;


						int milui_date_n = convert_date_milui(milui_date);//stoi(milui_date.substr(6, 4) + milui_date.substr(3, 2) + milui_date.substr(0, 2));
						int bidud_date_n = milui_date_n;
						if (bidud_date != "") {
							bidud_date_n = stoi(bidud_date.substr(6, 4) + bidud_date.substr(3, 2) + bidud_date.substr(0, 2));
						}

						map_qa_milui_date_n[milui_date_n]++;

						//fprintf(stderr, "milui_date_n %i\n", milui_date_n);
						//getchar();


						article_milui_dates[milui_date_n]++;

						//fprintf(stderr, "milui_date_n %i\n", milui_date_n);
						//getchar();

						//fprintf(stderr, "i am here 3 \n");

						//run pre bidud
						//milui_date_n = bidud_date_n;

						int corona_ind = -9;
						int corona_date = -9;
						int corona_diff = -9;
						int corona_diff_bidud = -9;
						if (corona_pos_final.find(id) != corona_pos_final.end()) {
							corona_ind = 1;
							corona_date = corona_pos_final[id];
							corona_diff = datediff(corona_date, milui_date_n);
							corona_diff_bidud = datediff(corona_date, bidud_date_n);
						}
						else if (test_neg_first.find(id) != test_neg_first.end()) {
							corona_ind = 0;
							corona_date = test_neg_first[id];
							corona_diff = datediff(corona_date, milui_date_n);
							corona_diff_bidud = datediff(corona_date, bidud_date_n);
						}
						else {
							corona_ind = -9;
							int idx = rand() % n_vec_neg_dates;
							corona_date = vec_neg_dates[idx];
							corona_diff = datediff(corona_date, milui_date_n);
							corona_diff_bidud = datediff(corona_date, bidud_date_n);
						}



						//fprintf(stderr, "i am here 4 \n");

						id_corona_ind[id] = corona_ind;

						
						//fprintf(stderr, "%s\n", fields[13].c_str());
						map_qa[fields[13]]++;
						

						int bidud_from_chul = stoib(fields[10]);
						int bidud_from_corona_patient = stoib(fields[11]);
						int bidud_from_tasminim = stoib(fields[12]);
						int bidud_from_personal = stoib(fields[13]);
						int bidud_other = stoib(fields[14]);





						//run pre...
						if (mm == 2) {
							
							//if (corona_ind == 1) {
								//fprintf(stderr, "corona_diff %i milui_date_n %i corona_date %i corona_ind %i", corona_diff, milui_date_n, corona_date, corona_ind);
								//getchar();
							//}

							if (corona_diff > 0 || corona_diff < -21) continue;

							//if (corona_diff > 0 ) continue;
						}
						else if (mm == 4) {
							if (corona_diff > 0) continue;
						}
						if (mm == 5) {
							//if (corona_diff <=0 || corona_diff>21) continue;
							if (corona_diff <= 0) continue;

							//if (corona_diff > 0 ) continue;
						}
						//continue;


						//run_pre_bidud
						//if (corona_diff > 0) continue;

						int smoking_current = 0, smoking_never = 0, smoking_ex = 0;

						//if (stoi(fields[58])==0) smoking_never=1;
						//else if (stoi(fields[58]) == 1) smoking_current = 1;
						//else if (stoi(fields[58]) == 2 || stoi(fields[58]) == 3) smoking_ex = 1;


						//fprintf(stderr, "%i\n", id);

						int symp_godesh_nazelet = stoib(fields[16]) + stoib(fields[32]); if (symp_godesh_nazelet > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_godesh_nazelet");
						//fprintf(stderr, "%i\n", id);
						int symp_keev_garon = stoib(fields[17]) + stoib(fields[33]); if (symp_keev_garon > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_keev_garon");
						int symp_shiul_yavesh = stoib(fields[18]) + stoib(fields[34]); if (symp_shiul_yavesh > 0) { map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul_yavesh"); map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul"); }
						int symp_shiul_leicha = stoib(fields[19]) + stoib(fields[35]); if (symp_shiul_leicha > 0) { map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul_leicha"); map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul"); }
						int symp_kotzer_neshima = stoib(fields[20]) + stoib(fields[36]); if (symp_kotzer_neshima > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_kotzer_neshima");
						int symp_keev_shririm = stoib(fields[21]) + stoib(fields[37]); if (symp_keev_shririm > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_keev_shririm");
						int symp_keev_rosh = stoib(fields[22]) + stoib(fields[38]); if (symp_keev_rosh > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_keev_rosh");
						int symp_ayefut = stoib(fields[23]) + stoib(fields[39]); if (symp_ayefut > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_ayefut");
						int symp_taam_reach = stoib(fields[24]) + stoib(fields[40]); if (symp_taam_reach > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_taam_reach");
						int symp_shilshul = stoib(fields[25]) + stoib(fields[41]); if (symp_shilshul > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_shilshul");
						int symp_bchilot_akahot = stoib(fields[26]) + stoib(fields[42]); if (symp_bchilot_akahot > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_bchilot_akahot");
						int symp_zmarmoret = stoib(fields[27]) + stoib(fields[43]); if (symp_zmarmoret > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_zmarmoret");
						int symp_bilbul = stoib(fields[28]) + stoib(fields[44]); if (symp_bilbul > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_bilbul");
						//int symp_none = stoib(fields[29]); if (symp_none > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_not_one_of_the_list");
						int symp_none = stoib(fields[29]); if (symp_none > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_none");
						int symp_other = stoib(fields[30]) + stoib(fields[45]); if (symp_other > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_other");
						//int symp_no = stoib(fields[31]); if (symp_no > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_no");

						//fprintf(stderr, "read %i %i ", count, 3);

						string chom_value = fields[46];
						if (chom_value == "chom_less_36" || chom_value == "chom_36_369" || chom_value == "chom_37_374") {
							map_id_date_all_symp[id][milui_date_n].push_back("chom_up_to_374");
							map_chom["chom_up_to_374"]++;
						}
						else if (chom_value == "chom_375_379") {
							map_id_date_all_symp[id][milui_date_n].push_back("chom_375_379");
							//map_id_date_all_symp[id][milui_date_n].push_back("chom_375_40");
							map_chom["chom_375_379"]++;
						}
						else if (chom_value == "chom_38_384" || chom_value == "chom_385_389" || chom_value == "chom_39_399" || chom_value == "chom_40") {
							map_id_date_all_symp[id][milui_date_n].push_back("chom_38_40");
							//map_id_date_all_symp[id][milui_date_n].push_back("chom_375_40");
							map_chom["chom_38_40"]++;
						}
						else if (symp_none > 0) {
							//map_id_date_all_symp[id][milui_date_n].push_back("chom_up_to_374");
							count_fix_chom++;
							map_chom["chom_up_to_374_fix"]++;
						}
						else {
							map_chom["no_value"]++;
						}

						if (map_id_date_all_symp[id][milui_date_n].size() == 0) {
							count_milui_out++;
							//if (corona_ind == 1) {
								//fprintf(stderr, "pos out .... %i %i \n", id, corona_date);
								//getchar();
							//}
							continue;
						}
						else {
							count_milui_in++;
						}


						if (corona_diff <= 0) {
							article_all_before[corona_ind][id]++;
						}
						else {
							article_all_after[corona_ind][id]++;
						}


						article_age_all[id] = age;

						if (bidud_from_chul == 1) {
							//fprintf(stderr, "%i %i \n", id, map_qa_chul[bidud_from_chul]);
							//getchar();
							map_qa_chul[id] = 1;;
						}
						//if (mm == 2 && corona_diff_bidud >= 0) {
						if (mm == 0) {
							if (bidud_from_chul == 1) id_bidud_from_chul[id] = 1;
							else if (id_bidud_from_chul.find(id) == id_bidud_from_chul.end()) id_bidud_from_chul[id] = bidud_from_chul;

							if (bidud_from_corona_patient == 1) id_bidud_from_corona_patient[id] = 1;
							else if (id_bidud_from_corona_patient.find(id) == id_bidud_from_corona_patient.end()) id_bidud_from_corona_patient[id] = bidud_from_corona_patient;

							if (bidud_from_tasminim == 1) id_bidud_from_tasminim[id] = 1;
							else if (id_bidud_from_tasminim.find(id) == id_bidud_from_tasminim.end()) id_bidud_from_tasminim[id] = bidud_from_tasminim;

							if (bidud_from_personal == 1) id_bidud_from_personal[id] = 1;
							else if (id_bidud_from_personal.find(id) == id_bidud_from_personal.end()) id_bidud_from_personal[id] = bidud_from_personal;

							if (bidud_other == 1) id_bidud_other[id] = 1;
							else if (id_bidud_other.find(id) == id_bidud_other.end()) id_bidud_other[id] = bidud_other;
						}




						//values...
						id_tzevet[id] = tzevet;

						if (smoking_current == 1) id_current_smoker[id] = 1; else if (id_current_smoker.find(id) == id_current_smoker.end()) id_current_smoker[id] = smoking_current;
						if (smoking_never == 1) id_never_smoker[id] = 1; else if (id_never_smoker.find(id) == id_never_smoker.end()) id_never_smoker[id] = smoking_never;
						if (smoking_ex == 1) id_ex_smoker[id] = 1; else if (id_ex_smoker.find(id) == id_ex_smoker.end()) id_ex_smoker[id] = smoking_ex;

						id_age[id] = age;
						id_num_of_quest[id]++;
						id_gender[id] = gender;

						//if (corona_date != -9) {
						if (corona_diff < 0) id_num_of_quest_before_test[id]++;
						else id_num_of_quest_after_or_equal_test[id]++;
						//}



						//split(fields, curr_line, boost::is_any_of(","));
						//for (int ii = 0; ii < fields.size(); ii++) {
							//string field_name = map_symp_names[ii];
							//string val = fields[ii];
							//map_symp_matrix[field_name].push_back(val);
						//}
					}



				}


				vector<float> temp_age_all;
				for (auto it = article_age_all.begin(); it != article_age_all.end(); it++) {
					temp_age_all.push_back((float)it->second);
				}
				float age_all_mean = get_mean(temp_age_all);
				float age_all_sdv = get_sdv(temp_age_all);
				fprintf(stderr, "table 1 article : age mean all : %f  sdv all %f  cunt all %i \n", age_all_mean, age_all_sdv, (int)temp_age_all.size());
				
				for (auto it = article_all_before.begin(); it != article_all_before.end(); it++) {
					int corona_ind = it->first;
					int count_ind = 0;
					for (auto it1 = article_all_before[corona_ind].begin(); it1 != article_all_before[corona_ind].end(); it1++) {
						int id = it1->first;
						int count = it1->second;
						count_ind += count;
					}
					fprintf(stderr, "table 1 id before ind %i countid  %i count_quest %i \n", it->first, (int)it->second.size(), count_ind);
				}

				for (auto it = article_all_after.begin(); it != article_all_after.end(); it++) {
					int corona_ind = it->first;
					int count_ind = 0;
					for (auto it1 = article_all_after[corona_ind].begin(); it1 != article_all_after[corona_ind].end(); it1++) {
						int id = it1->first;
						int count = it1->second;
						count_ind += count;
					}
					fprintf(stderr, "table 1 id after ind %i countid  %i count_quest %i \n", it->first, (int)it->second.size(), count_ind);
				}

				//return 1;

				//getchar();



				fprintf(stderr, "map_qa_chul  %i \n", (int) map_qa_chul.size());
				//fprintf(stderr, "map_qa_chul  %i \n", map_qa_chul[1]);
				//getchar();

				//return 1;

				fprintf(stderr, "count_milui_all : %i \n", count_milui_in + count_milui_out);
				fprintf(stderr, "count_milui_in : %i %f \n", count_milui_in, (float)count_milui_in / (float)(count_milui_in + count_milui_out));
				fprintf(stderr, "count_milui_out : %i %f \n", count_milui_out, (float)count_milui_out / (float)(count_milui_in + count_milui_out));

				//for (auto it = article_milui_dates.begin(); it != article_milui_dates.end(); it++) {
					//fprintf(stderr, "%i %i \n", it->first, it->second);
				//}


				for (auto it = map_qa1.begin(); it != map_qa1.end(); it++) {
					//fprintf(stderr, "%s\t%i\n", it->first.c_str(), it->second);
					fprintf(stderr, "%i\t%i\n", it->first, it->second);
				}
				//return 1;
				//fclose(fout);
				//for (auto it = map_qa_milui_date_n.begin(); it != map_qa_milui_date_n.end(); it++) {
					//fprintf(stderr, "%i %i \n", it->first, it->second);

				//}
				//getchar();

			}




			//return 1;

			//getchar();

			//***** create table 1 
			string fname;
			FILE* fout = NULL;

			fprintf(stderr, "i am here 1 ...\n");

			if (mm == 0) {
				fname = symp_dir + "data_quest_table1_id_level_time_all.txt";
				fout = fopen(fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\n", "param_name", "no_test", "corona_negative", "corona_positive");
			}
			else if (mm == 1) {
				fname = symp_dir + "data_quest_table1_quest_level_time_all.txt";
				fout = fopen(fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\n", "param_name", "no_test", "corona_negative", "corona_positive");
			}
			else if (mm == 2) {
				fname = symp_dir + "data_quest_table1_id_level_before_time_0_21_or.txt";
				fout = fopen(fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "no_test", "corona_negative"
					, "corona_positive", "noind_no_test", "noind_corona_negative", "noind_corona_positive"
					, "or_positive_negative", "CI", "or_positive_no_test", "CI", "or_negative_no_test", "CI"
					, "rr_positive_negative", "CI", "rr_positive_no_test", "CI", "rr_negative_no_test", "CI"
				);
			}
			else if (mm == 3) {
				fname = symp_dir + "data_quest_table1_id_level_time_all_or.txt";
				fout = fopen(fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "no_test", "corona_negative"
					, "corona_positive", "noind_no_test", "noind_corona_negative", "noind_corona_positive"
					, "or_positive_negative", "CI", "or_positive_no_test", "CI", "or_negative_no_test", "CI"
					, "rr_positive_negative", "CI", "rr_positive_no_test", "CI", "rr_negative_no_test", "CI"
				);
			}
			else if (mm == 4) {
				fname = symp_dir + "data_quest_table1_id_level_before_time_0_999_or.txt";
				fout = fopen(fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "no_test", "corona_negative"
					, "corona_positive", "noind_no_test", "noind_corona_negative", "noind_corona_positive"
					, "or_positive_negative", "CI", "or_positive_no_test", "CI", "or_negative_no_test", "CI"
					, "rr_positive_negative", "CI", "rr_positive_no_test", "CI", "rr_negative_no_test", "CI"
				);
			}
			else if (mm == 5) {
				fname = symp_dir + "data_quest_table1_id_level_after_time_0_999_or.txt";
				fout = fopen(fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "no_test", "corona_negative"
					, "corona_positive", "noind_no_test", "noind_corona_negative", "noind_corona_positive"
					, "or_positive_negative", "CI", "or_positive_no_test", "CI", "or_negative_no_test", "CI"
					, "rr_positive_negative", "CI", "rr_positive_no_test", "CI", "rr_negative_no_test", "CI"
				);
			}



			fprintf(stderr, "i am here 2 ...\n");

			map<string, int> map_symp_names1;
			for (auto it = map_id_date_all_symp.begin(); it != map_id_date_all_symp.end(); it++) {
				int numerator = it->first;
				for (auto it1 = map_id_date_all_symp[numerator].begin(); it1 != map_id_date_all_symp[numerator].end(); it1++) {
					int milui_date = it1->first;
					for (int jj = 0; jj < map_id_date_all_symp[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_all_symp[numerator][milui_date][jj];
						map_symp_names1[signal] = 1;
					}
				}
			}

			fprintf(stderr, "==== i am here 3 ...\n");


			//id level
			if (mm == 0 || mm == 2 || mm == 3 || mm == 4 || mm == 5) {
				map<int, map<string, map<int, int>>> corona_count_param;
				map<int, map<string, map<int, vector<float>>>> corona_avg_param;

				fprintf(stderr, "i am here 31 ...\n");

				map<int, int> map_count_id;
				map<int, int> map_never_smoker_id;
				map<int, int> map_age_18_40_id;

				for (auto it = id_num_of_quest.begin(); it != id_num_of_quest.end(); it++) {
					int count_param = 0;
					int id = it->first;
					int corona_ind = id_corona_ind[id];
					int age = id_age[id];
					int gender = id_gender[id];
					int num_of_quest = it->second;
					int current_smoker = id_current_smoker[id];
					int ex_smoker = id_ex_smoker[id];
					int never_smoker = id_never_smoker[id];
					int number_of_quest_before = 0; if (id_num_of_quest_before_test.find(id) != id_num_of_quest_before_test.end()) number_of_quest_before = id_num_of_quest_before_test[id];
					int number_of_quest_after = 0; if (id_num_of_quest_after_or_equal_test.find(id) != id_num_of_quest_after_or_equal_test.end()) number_of_quest_after = id_num_of_quest_after_or_equal_test[id];

					int info_ind = 0;
					if (segal_info.find(id) != segal_info.end()) info_ind = 1;


					corona_count_param[count_param++]["count_id"][corona_ind]++;
					corona_count_param[count_param++]["count_id_info"][corona_ind]+= info_ind;
					map_count_id[corona_ind]++;

					if (never_smoker == 1) {
						map_never_smoker_id[corona_ind]++;
					}

					if (mm != 2 && mm != 3 && mm != 4 && mm != 5) {
						corona_count_param[count_param++]["count_quest"][corona_ind] += num_of_quest;
						corona_count_param[count_param++]["count_quest_before_test"][corona_ind] += number_of_quest_before;
					}

					if (mm == 0)
						corona_count_param[count_param++]["count_quest_after_or_equal_test"][corona_ind] += number_of_quest_after;


					if (gender == 1) corona_count_param[count_param]["count_female"][corona_ind] ++; count_param++;
					if (gender == 0) corona_count_param[count_param]["count_male"][corona_ind] ++; count_param++;

					if (current_smoker == 1) corona_count_param[count_param + 100]["count_current_smoker"][corona_ind] ++;
					else if (ex_smoker == 1) corona_count_param[count_param + 101]["count_ex_smoker"][corona_ind] ++;
					else if (never_smoker == 1) corona_count_param[count_param + 102]["count_never_smoker"][corona_ind] ++;

					if (age <= 40) {
						corona_count_param[count_param + 103]["age_18_40"][corona_ind] ++;
						map_age_18_40_id[corona_ind]++;
					}
					else if (age <= 60) corona_count_param[count_param + 104]["age_40_60"][corona_ind] ++;
					else if (age <= 100) corona_count_param[count_param + 105]["age_60_100"][corona_ind] ++;


					//fprintf(stderr, "count_param %i \n", count_param);
					//getchar();

					if (mm == 0) {
						if (id_bidud_from_chul[id] == 1)
							corona_count_param[count_param]["count_bidud_from_chul"][corona_ind] ++;
						count_param++;
						if (id_bidud_from_corona_patient[id] == 1)
							corona_count_param[count_param]["count_bidud_from_corona_patient"][corona_ind] ++;
						count_param++;
						if (id_bidud_from_tasminim[id] == 1)
							corona_count_param[count_param]["count_bidud_from_tasminim"][corona_ind] ++;
						count_param++;
						if (id_bidud_from_personal[id] == 1)
							corona_count_param[count_param]["count_bidud_from_personal"][corona_ind] ++;
						count_param++;
						if (id_bidud_other[id] == 1)
							corona_count_param[count_param]["count_bidud_other"][corona_ind] ++;
						count_param++;

						//fprintf(stderr, "count_param %i \n", count_param);
						//getchar();

						corona_avg_param[count_param]["avg_sdv_num_of_quest"][3].push_back((float)num_of_quest);
						corona_avg_param[count_param]["avg_sdv_num_of_quest"][corona_ind].push_back((float)num_of_quest);
						count_param++;
						corona_avg_param[count_param]["avg_sdv_num_of_age"][corona_ind].push_back((float)age);
						count_param++;
					}

					for (auto it2 = map_symp_names1.begin(); it2 != map_symp_names1.end(); it2++) {
						string symp_to_check = it2->first;
						int ind = 0;
						for (auto it1 = map_id_date_all_symp[id].begin(); it1 != map_id_date_all_symp[id].end(); it1++) {
							int milui_date = it1->first;
							for (int jj = 0; jj < map_id_date_all_symp[id][milui_date].size(); jj++) {
								string signal = map_id_date_all_symp[id][milui_date][jj];
								if (signal == symp_to_check) ind = 1;
							}
						}
						corona_count_param[count_param][symp_to_check][corona_ind] += ind;
						count_param++;
					}

				}

				fprintf(stderr, "i am here 32 ...\n");


				for (auto it0 = corona_count_param.begin(); it0 != corona_count_param.end(); it0++) {
					int code = it0->first;
					for (auto it = corona_count_param[code].begin(); it != corona_count_param[code].end(); it++) {
						string param = it->first;
						fprintf(fout, "%s\t", param.c_str());
						int count_ind_no_test = 0, count_ind_negative = 0, count_ind_positive = 0, count_noind_no_test = 0, count_noind_negative = 0, count_noind_positive = 0;
						for (auto it1 = corona_count_param[code][param].begin(); it1 != corona_count_param[code][param].end(); it1++) {
							int corona_ind = it1->first;
							int count_id = it1->second;
							//fprintf(stderr, "%i\t%i\t", corona_ind, count_id);

							if (corona_ind == -9) {
								count_ind_no_test = count_id;
								count_noind_no_test = map_count_id[corona_ind] - count_ind_no_test;
								if (param == "count_current_smoker" || param == "count_ex_smoker") count_noind_no_test = map_never_smoker_id[corona_ind];
								if (param == "age_40_60" || param == "age_60_100") count_noind_no_test = map_age_18_40_id[corona_ind];
							}
							else if (corona_ind == 0) {
								count_ind_negative = count_id;
								count_noind_negative = map_count_id[corona_ind] - count_ind_negative;
								if (param == "count_current_smoker" || param == "count_ex_smoker") count_noind_negative = map_never_smoker_id[corona_ind];
								if (param == "age_40_60" || param == "age_60_100") count_noind_negative = map_age_18_40_id[corona_ind];
							}
							else if (corona_ind == 1) {
								count_ind_positive = count_id;
								count_noind_positive = map_count_id[corona_ind] - count_ind_positive;
								if (param == "count_current_smoker" || param == "count_ex_smoker") count_noind_positive = map_never_smoker_id[corona_ind];
								if (param == "age_40_60" || param == "age_60_100") count_noind_positive = map_age_18_40_id[corona_ind];
							}

							fprintf(fout, "%i\t", count_id);
						}

						if (mm == 2 || mm == 3 || mm == 4 || mm==5) {
							if (param != "count_id" && param != "count_never_smoker" && param != "age_18_40") {
								fprintf(fout, "%i\t%i\t%i\t", count_noind_no_test, count_noind_negative, count_noind_positive);
								vector<float>  or_positive_negative = get_or(count_ind_positive, count_noind_positive, count_ind_negative, count_noind_negative);
								vector<float>  or_positive_notest = get_or(count_ind_positive, count_noind_positive, count_ind_no_test, count_noind_no_test);
								vector<float>  or_negative_notest = get_or(count_ind_negative, count_noind_negative, count_ind_no_test, count_noind_no_test);
								fprintf(fout, "%.2f\t[%.2f-%.2f]\t", or_positive_negative[0], or_positive_negative[1], or_positive_negative[2]);
								fprintf(fout, "%.2f\t[%.2f-%.2f]\t", or_positive_notest[0], or_positive_notest[1], or_positive_notest[2]);
								fprintf(fout, "%.2f\t[%.2f-%.2f]\t", or_negative_notest[0], or_negative_notest[1], or_negative_notest[2]);

								//fprintf(stderr, "%s %i %i %i %i \n", param.c_str(),  count_ind_positive, count_noind_positive, count_ind_negative, count_noind_negative);
								//getchar();
								if ((count_noind_positive + count_noind_negative == 0) || (count_ind_positive + count_ind_negative == 0)) {
									fprintf(fout, "\t\t\t");
								}
								else {
									vector<float>  rr_positive_negative = get_rr(count_ind_positive, count_noind_positive, count_ind_negative, count_noind_negative);

									//fprintf(stderr, "param %s count_ind_positive %i count_noind_positive %i count_ind_negative %i count_noind_negative %i rr %f \n", param.c_str(),
										//count_ind_positive, count_noind_positive, count_ind_negative, count_noind_negative, rr_positive_negative[0]);
									//getchar();

									vector<float>  rr_positive_notest = get_rr(count_ind_positive, count_noind_positive, count_ind_no_test, count_noind_no_test);
									vector<float>  rr_negative_notest = get_rr(count_ind_negative, count_noind_negative, count_ind_no_test, count_noind_no_test);
									fprintf(fout, "%.2f\t[%.2f-%.2f]\t", rr_positive_negative[0], rr_positive_negative[1], rr_positive_negative[2]);
									fprintf(fout, "%.2f\t[%.2f-%.2f]\t", rr_positive_notest[0], rr_positive_notest[1], rr_positive_notest[2]);
									fprintf(fout, "%.2f\t[%.2f-%.2f]\t", rr_negative_notest[0], rr_negative_notest[1], rr_negative_notest[2]);
								}
							}
						}


						fprintf(fout, "\n");
					}
				}

				fprintf(stderr, "i am here 33 ...\n");

				for (auto it0 = corona_avg_param.begin(); it0 != corona_avg_param.end(); it0++) {
					int code = it0->first;

					for (auto it = corona_avg_param[code].begin(); it != corona_avg_param[code].end(); it++) {
						string param = it->first;
						fprintf(fout, "%s\t", param.c_str());
						for (auto it1 = corona_avg_param[code][param].begin(); it1 != corona_avg_param[code][param].end(); it1++) {
							int corona_ind = it1->first;
							float avg = get_mean(it1->second);
							float sdv = get_sdv(it1->second);
							fprintf(fout, "%.2f(%.2f)\t", avg, sdv);
						}
						fprintf(fout, "\n");
					}
				}

				fprintf(stderr, "i am here 34 ...\n");
			}

			fprintf(stderr, "i am here 4 ...\n");

			//quest level
			if (mm == 1) {
				map<int, map<string, map<int, int>>> corona_count_param;
				int count_param = 0;
				for (auto it2 = map_symp_names1.begin(); it2 != map_symp_names1.end(); it2++) {
					string symp_to_check = it2->first;
					for (auto it = map_id_date_all_symp.begin(); it != map_id_date_all_symp.end(); it++) {
						int id = it->first;
						int corona_ind = id_corona_ind[id];
						for (auto it1 = map_id_date_all_symp[id].begin(); it1 != map_id_date_all_symp[id].end(); it1++) {
							int milui_date = it1->first;
							for (int jj = 0; jj < map_id_date_all_symp[id][milui_date].size(); jj++) {
								string signal = map_id_date_all_symp[id][milui_date][jj];
								if (signal == symp_to_check) corona_count_param[count_param][symp_to_check][corona_ind]++;
							}
						}
					}
					count_param++;
				}
				fprintf(stderr, "quest level ...\n");
				for (auto it0 = corona_count_param.begin(); it0 != corona_count_param.end(); it0++) {
					int code = it0->first;
					for (auto it = corona_count_param[code].begin(); it != corona_count_param[code].end(); it++) {
						string param = it->first;
						fprintf(fout, "%s\t", param.c_str());
						for (auto it1 = corona_count_param[code][param].begin(); it1 != corona_count_param[code][param].end(); it1++) {
							int corona_ind = it1->first;
							int count_id = it1->second;
							fprintf(fout, "%i\t", count_id);
						}
						fprintf(fout, "\n");
					}
				}

			}


			fprintf(stderr, "i am here 5 ...\n");

			fclose(fout);
		}


	}


	if (CORONA_ARTICLE_MERGE_MACCABI == 1) {

		fprintf(stderr, "**** CORONA_ARTICLE_MERGE_MACCABI **** \n");

		string symp_dir = "C:\\corona_segal\\";
		string symp_fname = symp_dir + "corona_quest.txt";
		string tests = symp_dir + "tests.txt";
		string diag_fname = symp_dir + "Diagnosis.txt";




		string karni_kod_mapping_fname = symp_dir + "karni_symp_mapping.txt";
		string pop_fname_merge = symp_dir + "pop.txt";
		string hosp_fname = symp_dir + "hosp.txt";


		string symp_id_fname = symp_dir + "ALL_DIAGNOSIS_FROM.txt";  //row data
		string symp_desc_fname = symp_dir + "diag_code_desc_from_esma.txt";  //row data
		string smadar_mapping_icd_to_group = "smadar_mapping_icd2.txt";
		string smadar_mapping_groups_name = "smadar_mapping_groups.txt";


		map<int, int> segal_info;
		if (1 == 1) {
				int count = 0;
				string fname = "segal_info_indication.txt";
				fprintf(stderr, "read %s \n", fname.c_str());
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0)) {
						count++;
						if (count == 1) continue;
						split(fields, curr_line, boost::is_any_of("\t"));
						int id = stoi(fields[0]);
						int code = stoi(fields[1]);

						int ind = 0;
						if (code > 0) {
							segal_info[id] = 1;
						}
					}
				}
				fprintf(stderr, "segal_info %i\n", (int)segal_info.size());
		}
		//getchar();

		//string symp_id_fname = symp_dir + "maccabi_diag_from_medical_record_id_1406.txt";  //row data

		fprintf(stderr, "i am here 0 ... \n");

		for (int qq=0; qq<3; qq++) {

		int RUN_ON_QUEST = 0;
		int RUN_ON_DOCTOR = 0;

		string run_s = "";

		if (qq == 0 || qq == 2) {
			RUN_ON_QUEST = 1;
			run_s = "RUN_ON_QUEST";
		}
		if (qq == 1 || qq == 2) {
			RUN_ON_DOCTOR = 1;
			run_s = "RUN_ON_DOCTOR";
		}

		fprintf(stderr, "**** %s ******", run_s.c_str());

		//read tests
		map<int, int> test_neg_first;
		map<int, int> test_pos_first;
		map<int, int>  test_any;
		map<int, map<int, string>> id_all_tests;
		map<int, int> id_age;
		map<int, int> id_gender;
		if (1 == 1) {
			map<string, int> tests_names_map;
			int count = 0;
			string fname = tests;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int jj = 0; jj < fields.size(); jj++)
							tests_names_map[fields[jj]] = jj;
						continue;
					}

					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[tests_names_map["RANDOM_ID"]]);


					int age = stoi(fields[5]);
					id_age[id] = age;

					int gender = stoi(fields[6]);
					id_gender[id] = gender;
					

					string SAMPLE_RESULT_DATE = fields[tests_names_map["SAMPLE_RESULT_DATE"]];
					string execution_date = fields[tests_names_map["SAMPLE_EXECUTION_DATE"]];
					int TEST_RESULT_CD = stoi(fields[tests_names_map["TEST_RESULT_CD"]]);
					string final_date;
					if (execution_date == "1800-01-01 00:00:00.000" && SAMPLE_RESULT_DATE == "1/1/2020") continue;
					if (execution_date == "1800-01-01 00:00:00.000" && convert_date(SAMPLE_RESULT_DATE) == 20200101) continue;

					int new_date;
					if (execution_date != "1800-01-01 00:00:00.000" && convert_date(execution_date) <= datefilter) {
						final_date = execution_date;
						new_date = convert_date(final_date);
					}
					else {
						final_date = SAMPLE_RESULT_DATE;
						new_date = convert_date(final_date);
					}

					if (TEST_RESULT_CD == 1) {
						test_any[id] = 1;
						id_all_tests[id][new_date] = "corona_test_neg";
						if (test_neg_first.find(id) == test_neg_first.end())
							test_neg_first[id] = new_date;
						else if (new_date < test_neg_first[id]) test_neg_first[id] = new_date;
					}

					if (TEST_RESULT_CD == 2) {
						test_any[id] = 1;
						id_all_tests[id][new_date] = "corona_test_pos";
						if (test_pos_first.find(id) == test_pos_first.end())
							test_pos_first[id] = new_date;
						else if (new_date < test_pos_first[id]) test_pos_first[id] = new_date;
					}
				}
			}
		}

		//fprintf(stderr, "id_age : %i ", id_age.size());
		//getchar();

		vector<int> vec_neg_dates;
		for (auto it = test_neg_first.begin(); it != test_neg_first.end(); it++) {
			vec_neg_dates.push_back(it->second);
		}
		int n_vec_neg_dates = (int)vec_neg_dates.size();

		//read_diagnosis
		map<int, int> id_diagnosys_corona_date;
		if (1 == 2) {
			map<string, int> map_diagnosis_names;
			map<int, int>id_diagnosys_corona;

			int count = 0;
			string fname = diag_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int jj = 0; jj < fields.size(); jj++)
							map_diagnosis_names[fields[jj]] = jj;
						continue;
					}
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[map_diagnosis_names["RANDOM_ID"]]);
					int DIAGNOSIS_TYPE_CODE = stoi(fields[map_diagnosis_names["DIAGNOSIS_TYPE_CODE"]]);
					string VISIT_DATE = fields[map_diagnosis_names["VISIT_DATE"]];
					int VISIT_DATE_ki = convert_date(VISIT_DATE);

					if (DIAGNOSIS_TYPE_CODE == 3 && id_diagnosys_corona.find(id) == id_diagnosys_corona.end()) {
						id_diagnosys_corona[id] = 1;
						id_diagnosys_corona_date[id] = VISIT_DATE_ki;
					}
					else if (DIAGNOSIS_TYPE_CODE == 3 && id_diagnosys_corona.find(id) != id_diagnosys_corona.end()) {
						if (VISIT_DATE_ki < id_diagnosys_corona_date[id])  id_diagnosys_corona_date[id] = VISIT_DATE_ki;
					}
				}
			}
		}


		//read-pop_file 
		map<int, int> map_id_recover;
		map<int, int> map_id_death;
		if (1 == 1) {
			int count = 0;
			string fname = pop_fname_merge;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					int age = stoi(fields[4]);
					int gender = stoi(fields[5]);


					if (fields[2] == "1") {
						map_id_recover[id] = convert_date(fields[3]);
					}


					if (fields[7] != "12/31/9999") map_id_death[id] = convert_date(fields[7]);

				}
			}
		}
		fprintf(stderr, "recover size %i \n", (int)map_id_recover.size());
		fprintf(stderr, "death  size %i \n", (int)map_id_death.size());


		//hosp
		map<int, int> map_id_hosp;
		map<int, int> map_id_icu;
		map<int, int> map_id_death1;
		if (1 == 1) {
			int count = 0;
			string fname = hosp_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					int code= stoi(fields[19]);
					int icu = stoi(fields[20]);
					if (icu==1) map_id_icu[id] = 1;
					map_id_hosp[id] = 1;
					if (code == 4) map_id_death1[id] = 1;

				}
			}
		}
		//fprintf(stderr, "map_id_icu %i \n", (int)map_id_icu.size());
		//fprintf(stderr, "map_id_hosp %i \n", (int)map_id_hosp.size());
		//fprintf(stderr, "map_id_death1 %i \n", (int)map_id_death1.size());

		//return 1;

		fprintf(stderr, "i am here 1 ... \n");


		int count_only_diag = 0;
		int count_only_diag_and_recover = 0;
		int count_only_diag_and_hosp = 0;

		int count_only_test = 0;
		int count_only_test_and_recover = 0;
		int count_only_test_and_hosp = 0;


		map<int, int> corona_pos_final;
		for (auto it = test_pos_first.begin(); it != test_pos_first.end(); it++) {
			int id = it->first;
			int pos_date = it->second;
			corona_pos_final[id] = pos_date;

			count_only_test++;
			if (map_id_recover.find(id) != map_id_recover.end()) {
				count_only_test_and_recover++;
			}
			else if (map_id_hosp.find(id)!= map_id_hosp.end()){
				count_only_test_and_hosp++;
			}

			if (id_diagnosys_corona_date.find(id) != id_diagnosys_corona_date.end()) {
				if (id_diagnosys_corona_date[id] < pos_date) corona_pos_final[id] = id_diagnosys_corona_date[id];
			}
		}
		for (auto it = id_diagnosys_corona_date.begin(); it != id_diagnosys_corona_date.end(); it++) {
			int id = it->first;
			int diag_date = it->second;
			if (corona_pos_final.find(id) == corona_pos_final.end()) {
				count_only_diag++;

				if (map_id_recover.find(id) != map_id_recover.end()) {
					count_only_diag_and_recover++;
					corona_pos_final[id] = diag_date;
				} 
				else if (map_id_hosp.find(id) != map_id_hosp.end()) {
					count_only_diag_and_hosp++;
				}
			}
			if (test_any.find(id) == test_any.end()) test_any[id] = 1;
		}
		fprintf(stderr, "count_only_diag : %i\n", count_only_diag);
		fprintf(stderr, "count_only_diag_and_recover : %i\n", count_only_diag_and_recover);
		fprintf(stderr, "count_only_diag_and_hosp : %i\n", count_only_diag_and_hosp);

		fprintf(stderr, "count_only_test : %i\n", count_only_test);
		fprintf(stderr, "count_only_test_and_recover : %i\n", count_only_test_and_recover);
		fprintf(stderr, "count_only_test_and_hosp : %i\n", count_only_test_and_hosp);



		//fprintf(stderr, "corona_pos_final %i \n", (int)corona_pos_final.size());
		//getchar();

		map<int, int> id_corona_ind_new;
		for (auto it = corona_pos_final.begin(); it != corona_pos_final.end(); it++) {
			int id = it->first;
			id_corona_ind_new[id] = 1;
		}
		for (auto it = test_neg_first.begin(); it != test_neg_first.end(); it++) {
			int id = it->first;
			if (corona_pos_final.find(id)== corona_pos_final.end())
				id_corona_ind_new[id] =0;
		}


		

		//read karni group
		map<string, string> map_karni_group;
		if (1 == 1) {
			string fname = karni_kod_mapping_fname;
			int count = 0;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) continue;
					else {
						split(fields, curr_line, boost::is_any_of("\t"));
						string icd_code = fields[0];
						string maccabi_code = fields[1];
						string group = fields[2];
						trim(group);
						//fprintf(stderr, "%s\n", curr_line.c_str());
						//getchar();

						if (icd_code != "NULL" && group != "0") map_karni_group[icd_code] = group;
						if (maccabi_code != "NULL" && group != "0") map_karni_group[maccabi_code] = group;
					}
				}
			}
		}


		map<string, int> MAP_SYMP_ALL;

		//****** read doctor
		map<int, map<int, vector<string>>> id_date_diags;
		map<int, int> id_corona_ind;
		map<int, int> map_id_diag;
		map<int, map<int, int>> map_id_vistis;
		if (1==2) {
			int count = 0;
			int count_with_group = 0;
			int count_no_group = 0;
			string fname = symp_id_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					string symp_code = fields[1];
					int date = stoi(fields[2]);

					int corona_ind = -9;
					int corona_date = -9;
					int corona_diff = -9;
					if (corona_pos_final.find(id) != corona_pos_final.end()) {
						corona_ind = 1;
						corona_date = corona_pos_final[id];
						corona_diff = datediff(corona_date, date);
					}
					else if (test_neg_first.find(id) != test_neg_first.end()) {
						corona_ind = 0;
						corona_date = test_neg_first[id];
						corona_diff = datediff(corona_date, date);
					}
					else {
						continue;
					}

					id_corona_ind[id] = corona_ind;
					map_id_diag[id]++;
					map_id_vistis[id][date] = 1;




					string symp_group = "Other";
					if (map_karni_group.find(symp_code) != map_karni_group.end()) {
						symp_group = map_karni_group[symp_code];
						count_with_group++;
					}
					else {
						count_no_group++;
					}

					id_date_diags[id][date].push_back(symp_group);
					MAP_SYMP_ALL[symp_group] = 1;
				}
			}
			fprintf(stderr, "count_with_group %i\n", count_with_group);
			fprintf(stderr, "count_no_group %i\n", count_no_group);
		}


		//****** read doctor new
		if (1 == RUN_ON_DOCTOR) {

			map<string, string> map_icd_to_group_name;
			map<int, string> map_group_id_to_group_name;


			//read esma desc
			map<string, string> map_from_y_to_icd;
			map<string, string> map_icd_to_desc;

			//code desc
			if (1 == 1) {
				int count = 0;
				string fname = "C:\\corona_segal\\diag_code_desc_from_esma.txt";;
				fprintf(stderr, "read %s \n", fname.c_str());
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0)) {
						count++;
						if (count == 1) continue;
						split(fields, curr_line, boost::is_any_of("\t"));

						string DIAGNOSIS_CODE = fields[2];
						string DIAGNOSIS_DESC = fields[3];
						string DIAGNOSIS_CODE_Y = fields[4];

						map_from_y_to_icd[DIAGNOSIS_CODE_Y] = DIAGNOSIS_CODE;
						map_icd_to_desc[DIAGNOSIS_CODE] = DIAGNOSIS_DESC;


					}
				}
			}

			//smadar_mapping_groups_name
			if (1 == 1) {
				string fname = smadar_mapping_groups_name;
				int count = 0;
				fprintf(stderr, "read %s \n", fname.c_str());
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						split(fields, curr_line, boost::is_any_of("\t"));
						int group_id = stoi(fields[0]);
						string group_name = fields[1];
						trim(group_name);
						map_group_id_to_group_name[group_id] = group_name;
					}
				}
			}

			//smadar_mapping_icd_to_group
			if (1 == 1) {
				string fname = smadar_mapping_icd_to_group;
				int count = 0;
				fprintf(stderr, "read %s \n", fname.c_str());
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						split(fields, curr_line, boost::is_any_of("\t"));
						int group_id = stoi(fields[0]);
						string icd_code = fields[1];
						trim(icd_code);
						map_icd_to_group_name[icd_code] = map_group_id_to_group_name[group_id];
					}
				}
			}

			//read diags from doctor
			if (1 == 1) {
				int count = 0;
				int count_with_group = 0;
				int count_no_group = 0;
				string fname = symp_id_fname;
				fprintf(stderr, "read %s \n", fname.c_str());
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						count++;
						if (count == 1) continue;
						//split(fields, curr_line, boost::is_any_of("\t"));
						split(fields, curr_line, boost::is_any_of(","));
						int id = stoi(fields[0]);
						//int date = stoi(fields[2]);

						string symp_code = fields[1];
						trim(symp_code);

						string symp_code_icd = "";
						if (symp_code.substr(0, 1) == "Y") {
							if (map_from_y_to_icd.find(symp_code) == map_from_y_to_icd.end()) {
								fprintf(stderr, "cant find maccabi code %s \n", symp_code.c_str());
								getchar();
							}
							symp_code_icd = map_from_y_to_icd[symp_code];
						}
						else {
							symp_code_icd = symp_code;
						}


						string symp_code_icd_with_desc = "";
						if (map_icd_to_desc.find(symp_code_icd) == map_icd_to_desc.end()) {
							//fprintf(stderr, "cant find icd desc %s orig %s \n", symp_code_icd.c_str(), fields[1].c_str());
							//fprintf(stderr, " %s \n", curr_line.c_str());
							//getchar();
							symp_code_icd_with_desc = symp_code_icd;
						}
						else {
							symp_code_icd_with_desc = map_icd_to_desc[symp_code_icd];
						}


						//fprintf(stderr, "symp_code %s \n", symp_code.c_str());
						//getchar();

						int date = stoi(fields[2]);

						int corona_ind = -9;
						int corona_date = -9;
						int corona_diff = -9;
						if (corona_pos_final.find(id) != corona_pos_final.end()) {
							corona_ind = 1;
							corona_date = corona_pos_final[id];
							corona_diff = datediff(corona_date, date);
						}
						else if (test_neg_first.find(id) != test_neg_first.end()) {
							corona_ind = 0;
							corona_date = test_neg_first[id];
							corona_diff = datediff(corona_date, date);
						}
						else {
							continue;
						}

						id_corona_ind[id] = corona_ind;
						map_id_diag[id]++;
						map_id_vistis[id][date] = 1;


						if (map_icd_to_group_name.find(symp_code_icd) != map_icd_to_group_name.end()) {
							string symp_group = map_icd_to_group_name[symp_code_icd];
							id_date_diags[id][date].push_back(symp_group);
						}
					}
				}
				fprintf(stderr, "count_with_group %i\n", count_with_group);
				fprintf(stderr, "count_no_group %i\n", count_no_group);
			}
			fprintf(stderr, "map_id_diag : %i \n", (int)map_id_diag.size());

			



		}






		//****** read quest
		//read questionnaire sympton file
		map<string, int> quest_symp_list;
		map<string, vector<string>> map_symp_matrix;
		map<int, map<int, vector<string>>> map_id_date_all_symp;
		map<int, map<string, map<int, int>>> pini_age_gender;
		map<int, map<int, vector<string>>> map_id_bidud_vec;
		map<int, string> map_symp_names;
		//read questionnaire sympton file
		if (1 == RUN_ON_QUEST) {



			//string fname_out = symp_dir + "symp_with_corona.txt";
			//FILE *fout = fopen(fname_out.c_str(), "w");
			map<int, int> article_id;
			map<string, int> map_qa;
			map<int, int> map_qa1;
			map<int, int> map_uniq_id;
			map<int, int> map_uniq_id1;
			int count_milui_in = 0;
			int count_milui_out = 0;
			int count_fix_chom = 0;
			map<string, int> map_chom;
			map<int, int> article_milui_dates;
			int count = 0;
			string fname = symp_fname;

			fprintf(stderr, "======read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count % 100000 == 0) fprintf(stderr, "%i\n", count);
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int ii = 0; ii < fields.size(); ii++)
							map_symp_names[ii] = fields[ii];
						continue;
					}

					split(fields, curr_line, boost::is_any_of("\t"));
					//fprintf(stderr, "i am here 1 \n" );

					//if (count > 1000000) {
						//fprintf(stderr, "%s\n", fields[2].c_str());
					//}

					int id = stoi(fields[2]);

					map_uniq_id[id] = 1;


					int gender = stoi(fields[4]);
					int age = stoi(fields[3]);

					if (id_gender.find(id) == id_gender.end()) {
						id_gender[id] = gender;
					}


					if (id_age.find(id) == id_age.end()) {
						id_age[id] = age;
					}


					if (id_corona_ind_new.find(id) == id_corona_ind_new.end()) {
						id_corona_ind_new[id] = 2;
					}



					map_qa1[(int)fields.size()]++;

					//for (int mm = 10; mm < 20; mm++) {
					//}

					//fprintf(stderr, "%d", fields[11].substr(0, 1));
					//fprintf(stderr, "%d", fields[11].substr(1, 1));



					//getchar();




					//fprintf(stderr, "i am here 12 \n");

					string milui_date = fields[1];


					//if ((int)fields.size() != 59) {
						//fprintf(stderr, "%s\t%i\n", fields[0].c_str(), (int)fields.size());
						//getchar();
					//}



					//fprintf(stderr, "%s\n", fields[57].c_str());
					int tzevet = 0;//	if (fields[57]!="") stoib(fields[57]);

					//continue;


					//fprintf(stderr, "i am here 14 \n");
					//int age = stoi(fields[3]);
					//if (id_age.find(id) == id_age.end()) {
						//id_age[id] = age;
					//}



					//fprintf(stderr, "i am here 15 \n");
					//int gender = stoi(fields[4]);


					//fprintf(stderr, "i am here 16 \n");
					string bidud_date = fields[9];


					//fprintf(stderr, "i am here 2 \n");

					//if (gender == 0) continue;

					//vector<string> milui_date_vec;


					int milui_date_n = convert_date_milui(milui_date);//stoi(milui_date.substr(6, 4) + milui_date.substr(3, 2) + milui_date.substr(0, 2));
					int bidud_date_n = milui_date_n;
					if (bidud_date != "") {
						bidud_date_n = stoi(bidud_date.substr(6, 4) + bidud_date.substr(3, 2) + bidud_date.substr(0, 2));
					}


					for (int mm=5; mm<=14; mm++)
						map_id_bidud_vec[id][milui_date_n].push_back(fields[mm]);


					article_milui_dates[milui_date_n]++;

					//fprintf(stderr, "milui_date_n %i\n", milui_date_n);
					//getchar();

					//fprintf(stderr, "i am here 3 \n");

					//run pre bidud
					//milui_date_n = bidud_date_n;

					int corona_ind = -9;
					int corona_date = -9;
					int corona_diff = -9;
					int corona_diff_bidud = -9;
					if (corona_pos_final.find(id) != corona_pos_final.end()) {
						corona_ind = 1;
						corona_date = corona_pos_final[id];
						corona_diff = datediff(corona_date, milui_date_n);
						corona_diff_bidud = datediff(corona_date, bidud_date_n);
					}
					else if (test_neg_first.find(id) != test_neg_first.end()) {
						corona_ind = 0;
						corona_date = test_neg_first[id];
						corona_diff = datediff(corona_date, milui_date_n);
						corona_diff_bidud = datediff(corona_date, bidud_date_n);
					}
					else {
						corona_ind = -9;
						int idx = rand() % n_vec_neg_dates;
						corona_date = vec_neg_dates[idx];
						corona_diff = datediff(corona_date, milui_date_n);
						corona_diff_bidud = datediff(corona_date, bidud_date_n);
					}



					//fprintf(stderr, "i am here 4 \n");

					id_corona_ind[id] = corona_ind;


					//fprintf(stderr, "%s\n", fields[13].c_str());
					map_qa[fields[13]]++;


					int bidud_from_chul = stoib(fields[10]);
					int bidud_from_corona_patient = stoib(fields[11]);
					int bidud_from_tasminim = stoib(fields[12]);
					int bidud_from_personal = stoib(fields[13]);
					int bidud_other = stoib(fields[14]);



					//run_pre_bidud
					//if (corona_diff > 0) continue;

					int smoking_current = 0, smoking_never = 0, smoking_ex = 0;

					//if (stoi(fields[58])==0) smoking_never=1;
					//else if (stoi(fields[58]) == 1) smoking_current = 1;
					//else if (stoi(fields[58]) == 2 || stoi(fields[58]) == 3) smoking_ex = 1;


					//fprintf(stderr, "%i\n", id);

					int symp_godesh_nazelet = stoib(fields[16]) + stoib(fields[32]); if (symp_godesh_nazelet > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_godesh_nazelet");
					//fprintf(stderr, "%i\n", id);
					int symp_keev_garon = stoib(fields[17]) + stoib(fields[33]); if (symp_keev_garon > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_keev_garon");
					int symp_shiul_yavesh = stoib(fields[18]) + stoib(fields[34]); if (symp_shiul_yavesh > 0) { map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul_yavesh"); map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul"); }
					int symp_shiul_leicha = stoib(fields[19]) + stoib(fields[35]); if (symp_shiul_leicha > 0) { map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul_leicha"); map_id_date_all_symp[id][milui_date_n].push_back("symp_shiul"); }
					int symp_kotzer_neshima = stoib(fields[20]) + stoib(fields[36]); if (symp_kotzer_neshima > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_kotzer_neshima");
					int symp_keev_shririm = stoib(fields[21]) + stoib(fields[37]); if (symp_keev_shririm > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_keev_shririm");
					int symp_keev_rosh = stoib(fields[22]) + stoib(fields[38]); if (symp_keev_rosh > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_keev_rosh");
					int symp_ayefut = stoib(fields[23]) + stoib(fields[39]); if (symp_ayefut > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_ayefut");
					int symp_taam_reach = stoib(fields[24]) + stoib(fields[40]); if (symp_taam_reach > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_taam_reach");
					int symp_shilshul = stoib(fields[25]) + stoib(fields[41]); if (symp_shilshul > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_shilshul");
					int symp_bchilot_akahot = stoib(fields[26]) + stoib(fields[42]); if (symp_bchilot_akahot > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_bchilot_akahot");
					int symp_zmarmoret = stoib(fields[27]) + stoib(fields[43]); if (symp_zmarmoret > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_zmarmoret");
					int symp_bilbul = stoib(fields[28]) + stoib(fields[44]); if (symp_bilbul > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_bilbul");
					//int symp_none = stoib(fields[29]); if (symp_none > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_not_one_of_the_list");
					int symp_none = stoib(fields[29]); if (symp_none > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_none");
					int symp_other = stoib(fields[30]) + stoib(fields[45]); if (symp_other > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_other");
					//int symp_no = stoib(fields[31]); if (symp_no > 0) map_id_date_all_symp[id][milui_date_n].push_back("symp_no");

					//fprintf(stderr, "read %i %i ", count, 3);

					string chom_value = fields[46];
					if (chom_value == "chom_less_36" || chom_value == "chom_36_369" || chom_value == "chom_37_374") {
						map_id_date_all_symp[id][milui_date_n].push_back("chom_up_to_374");
						map_chom["chom_up_to_374"]++;
					}
					else if (chom_value == "chom_375_379") {
						map_id_date_all_symp[id][milui_date_n].push_back("chom_375_379");
						//map_id_date_all_symp[id][milui_date_n].push_back("chom_375_40");
						map_chom["chom_375_379"]++;
					}
					else if (chom_value == "chom_38_384" || chom_value == "chom_385_389" || chom_value == "chom_39_399" || chom_value == "chom_40") {
						map_id_date_all_symp[id][milui_date_n].push_back("chom_38_40");
						//map_id_date_all_symp[id][milui_date_n].push_back("chom_375_40");
						map_chom["chom_38_40"]++;
					}
					else if (symp_none > 0) {
						//map_id_date_all_symp[id][milui_date_n].push_back("chom_up_to_374");
						count_fix_chom++;
						map_chom["chom_up_to_374_fix"]++;
					}
					else {
						map_chom["no_value"]++;
					}

					if (map_id_date_all_symp[id][milui_date_n].size() == 0) {
						count_milui_out++;
						//fprintf(stderr, "")
						//if (corona_ind == 1) {
							//fprintf(stderr, "pos out .... %i %i \n", id, corona_date);
							//getchar();
						//}
						continue;
					}
					else {
						count_milui_in++;
					}


					map_uniq_id1[id] = 1;


					//values...
					//id_tzevet[id] = tzevet;

					//if (smoking_current == 1) id_current_smoker[id] = 1; else if (id_current_smoker.find(id) == id_current_smoker.end()) id_current_smoker[id] = smoking_current;
					//if (smoking_never == 1) id_never_smoker[id] = 1; else if (id_never_smoker.find(id) == id_never_smoker.end()) id_never_smoker[id] = smoking_never;
					//if (smoking_ex == 1) id_ex_smoker[id] = 1; else if (id_ex_smoker.find(id) == id_ex_smoker.end()) id_ex_smoker[id] = smoking_ex;

					//id_age[id] = age;
					//id_num_of_quest[id]++;
					//id_gender[id] = gender;

					//if (corona_date != -9) {
					//if (corona_diff < 0) id_num_of_quest_before_test[id]++;
					//else id_num_of_quest_after_or_equal_test[id]++;
					//}



					//split(fields, curr_line, boost::is_any_of(","));
					//for (int ii = 0; ii < fields.size(); ii++) {
						//string field_name = map_symp_names[ii];
						//string val = fields[ii];
						//map_symp_matrix[field_name].push_back(val);
					//}
				}
			}

			//return 1;
			fprintf(stderr, "map_uniq_id : %i\n", (int) map_uniq_id.size());
			fprintf(stderr, "map_uniq_id1 : %i\n", (int)map_uniq_id1.size());
			//getchar();

			fprintf(stderr, "count_milui_all : %i \n", count_milui_in + count_milui_out);
			fprintf(stderr, "count_milui_in : %i %f \n", count_milui_in, (float)count_milui_in / (float)(count_milui_in + count_milui_out));
			fprintf(stderr, "count_milui_out : %i %f \n", count_milui_out, (float)count_milui_out / (float)(count_milui_in + count_milui_out));

			//for (auto it = article_milui_dates.begin(); it != article_milui_dates.end(); it++) {
				//fprintf(stderr, "%i %i \n", it->first, it->second);
			//}


			//for (auto it = map_qa1.begin(); it != map_qa1.end(); it++) {
				//fprintf(stderr, "%s\t%i\n", it->first.c_str(), it->second);
				//fprintf(stderr, "%i\t%i\n", it->first, it->second);
			//}
			//return 1;
			//fclose(fout);
		}


		//FILE* fout_qa = fopen("qa.txt","w");
		//for (int mm = 0; mm < map_symp_names.size(); mm++)
			//fprintf(fout_qa, "%i\t%s\n",mm, map_symp_names[mm].c_str());
		//fclose(fout_qa);

		//return 1;
		//getchar();


		//join 
		//FILE *fout = fopen("quest_data.txt", "w");
		map<int, vector<int>> id_vec_dates;
		map<int, int> id_n_dates;
		map<int, int> id_n_dates_before_test;
		map<int, int> id_n_symp;
		map<int, map<string, int>> id_n_symp_uniq;
		map<int, map<string, int>> id_n_symp_uniq_good;
		map<string, int> map_symp_names1;
		map<int, map<int, vector<string>>> map_id_date_join;
		map<int, int> diag_from_quest;
		map<int, int> diag_from_doctor;

		if (1 == 1) {
			map<int, map<int, map<string, int>>> map_id_date_join_pre;
			for (auto it = map_id_date_all_symp.begin(); it != map_id_date_all_symp.end(); it++) {
				int numerator = it->first;
				//diag_from_quest[numerator] = 1;
				for (auto it1 = map_id_date_all_symp[numerator].begin(); it1 != map_id_date_all_symp[numerator].end(); it1++) {
					int milui_date = it1->first;
					for (int jj = 0; jj < map_id_date_all_symp[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_all_symp[numerator][milui_date][jj];
						//if (signal == "chom_375_379" || signal == "chom_38_40") signal = "symp_fever";

						map_id_date_join_pre[numerator][milui_date][signal] = 1;
						diag_from_quest[numerator] = 1;
					}
				}
			}

			fprintf(stderr, "map_id_date_join_pre  %i\n", (int)map_id_date_join_pre.size());
			


			for (auto it = id_date_diags.begin(); it != id_date_diags.end(); it++) {
				int numerator = it->first;
				diag_from_doctor[numerator] = 1;
				for (auto it1 = id_date_diags[numerator].begin(); it1 != id_date_diags[numerator].end(); it1++) {
					int milui_date = it1->first;
					for (int jj = 0; jj < id_date_diags[numerator][milui_date].size(); jj++) {
						string signal = id_date_diags[numerator][milui_date][jj];
						map_id_date_join_pre[numerator][milui_date][signal] = 1;
					}
				}
			}
			for (auto it = map_id_date_join_pre.begin(); it != map_id_date_join_pre.end(); it++) {
				int numerator = it->first;
				for (auto it1 = map_id_date_join_pre[numerator].begin(); it1 != map_id_date_join_pre[numerator].end(); it1++) {
					int milui_date = it1->first;
					id_n_dates[numerator]++;
					id_vec_dates[numerator].push_back(milui_date);
					if (corona_pos_final.find(numerator) != corona_pos_final.end()) {
						if (milui_date < corona_pos_final[numerator]) id_n_dates_before_test[numerator]++;
					}


					for (auto it2 = map_id_date_join_pre[numerator][milui_date].begin(); it2 != map_id_date_join_pre[numerator][milui_date].end(); it2++) {
						string signal = it2->first;
						map_symp_names1[signal];
						id_n_symp[numerator]++;
						id_n_symp_uniq[numerator][signal] = 1;
						map_id_date_join[numerator][milui_date].push_back(signal);

						if (signal != "symp_no" && signal != "symp_none" && signal != "chom_up_to_374") {
							id_n_symp_uniq_good[numerator][signal] = 1;
						}

					}
				}
			}
		}

		map<int, int>  diag_both;
		for (auto it = diag_from_doctor.begin(); it != diag_from_doctor.end(); it++) {
			int id = it->first;
			if (diag_from_quest.find(id) != diag_from_quest.end()) diag_both[id] = 1;
		}

		fprintf(stderr, "doctor : id in whole cohort : %i \n", (int)diag_from_doctor.size());
		fprintf(stderr, "quest : id in whole cohort : %i \n", (int)diag_from_quest.size());
		fprintf(stderr, "all : id in whole cohort : %i \n", (int)id_n_symp.size());

		int count_pos = 0; int count_neg = 0, count_no = 0;
		for (auto it = id_n_symp.begin(); it != id_n_symp.end(); it++) {
			int id = it->first;
			if (id_corona_ind_new[id] == 2) count_no++;
			else if (id_corona_ind_new[id] == 1)count_pos++;
			else count_neg++;
		}
		fprintf(stderr, "count_pos %i\n", count_pos);
		fprintf(stderr, "count_neg %i\n", count_neg);
		fprintf(stderr, "count_no %i\n", count_no);
		//return 1;


		fprintf(stderr, "both : id in whole cohort : %i \n", (int)diag_both.size());
		fprintf(stderr, "map_id_date_join  %i\n", (int)map_id_date_join.size());
		//return 1;


		//****** article ids before *******/
		if (1 == 1) {

			map<int, int> map_id_before;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int numerator = it->first;
				if (id_corona_ind_new[numerator] != 1) continue;
				for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {

					int milui_date = it1->first;
					int diff = datediff(milui_date, corona_pos_final[numerator]);
					if (diff >= 0 && diff <= 21)
						map_id_before[numerator] = 1;
				}
			}
			fprintf(stderr, "map_id_before  %i\n", (int) map_id_before.size());
			getchar();
		}


		//**** print id doctor ******/
		if (1 == 2) {
			FILE* fout = fopen("samples_for_hagai.txt", "w");
			fprintf(fout, "%s\t%s\t%s\n", "numerator", "milui_date", "signal");
			map<int, int> map_id_to_hagai;
			map_id_to_hagai[3057216] = 1;
			map_id_to_hagai[896430] = 1;
			map_id_to_hagai[1307129] = 1;
			map_id_to_hagai[3105875] = 1;
			map_id_to_hagai[157525] = 1;
			map_id_to_hagai[233994] = 1;
			map_id_to_hagai[479077] = 1;
			map_id_to_hagai[1283813] = 1;
			map_id_to_hagai[2168427] = 1;
			map_id_to_hagai[2426879] = 1;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int numerator = it->first;
				if (map_id_to_hagai.find(numerator) == map_id_to_hagai.end()) continue;
				for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {

					int milui_date = it1->first;

					for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_join[numerator][milui_date][jj];
						fprintf(fout, "%i\t%i\t%s\n", numerator, milui_date, signal.c_str());
					}
				}
			}
			fclose(fout);
		}


		//****** table 1 stats *****/
		if (1 == 2) {
			map<int, int> table_1_ind;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				int corona_ind = id_corona_ind_new[id];
				table_1_ind[corona_ind]++;
			}

			for (auto it = table_1_ind.begin(); it != table_1_ind.end(); it++)
				fprintf(stderr, "%i %i  \n", it->first, it->second);
		}
		//return 1;


		//****** combinations ****/
		if (1 ==2) {
			string fname_comb = "C:\\corona_segal\\symptom_combinations_";
			if (qq == 0) {
				fname_comb = fname_comb + "quest.csv";
			}
			else if (qq == 1) {
				fname_comb = fname_comb + "doctor.csv";
			}
			else {
				fname_comb = fname_comb + "quest_doctor.csv";
			}

			fprintf(stderr, "%s\n", fname_comb.c_str());



			
			FILE* fout_comb = fopen(fname_comb.c_str(), "w");


			fprintf(fout_comb, "%s,%s,", "level_of_grouping", "corona_ind");
			for (auto it3 = map_symp_names1.begin(); it3 != map_symp_names1.end(); it3++) 
				fprintf(fout_comb, "%s,", it3->first.c_str());
			fprintf(fout_comb, "%s\n", "count");



			//map_symp_names1
			map<int, map<string, int>> map_combinations;
			map<int, map<string, int>> map_combinations_id_level;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int numerator = it->first;
				int corona_ind = id_corona_ind_new[numerator];
				map<string, int> map_numerator_sig;
				for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
					int milui_date = it1->first;
					map<string, int> map_milui_sig;
					for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_join[numerator][milui_date][jj];
						map_milui_sig[signal] = 1;
						map_numerator_sig[signal] = 1;
					}

					string comb="";
					for (auto it3 = map_symp_names1.begin(); it3 != map_symp_names1.end(); it3++) {
						string sig_to_check = it3->first;
						int ind = 0;
						if (map_milui_sig.find(sig_to_check) != map_milui_sig.end()) ind = 1;
						comb = comb + to_string(ind) + "#";
					}
					map_combinations[corona_ind][comb]++;
				}

				string comb_id = "";
				for (auto it3 = map_symp_names1.begin(); it3 != map_symp_names1.end(); it3++) {
					string sig_to_check = it3->first;
					int ind = 0;
					if (map_numerator_sig.find(sig_to_check) != map_numerator_sig.end()) ind = 1;
					comb_id = comb_id + to_string(ind) + "#";
				}
				map_combinations_id_level[corona_ind][comb_id]++;


			}

			//**** quest level 
			for (auto it = map_combinations.begin(); it != map_combinations.end(); it++) {
				int corona_ind = it->first;
				for (auto it1 = map_combinations[corona_ind].begin(); it1 != map_combinations[corona_ind].end(); it1++) {
					string comb = it1->first;
					int count = it1->second;
					vector<string> fields;
					split(fields, comb, boost::is_any_of("#"));
					fprintf(fout_comb, "%s,%i,", "quest_level", corona_ind);
					for (int jj = 0; jj < fields.size(); jj++) {
						if (fields[jj]!="") fprintf(fout_comb, "%s,", fields[jj].c_str());
					}
					fprintf(fout_comb, "%i\n", count);
				}
			}

			//**** id _level 
			for (auto it = map_combinations_id_level.begin(); it != map_combinations_id_level.end(); it++) {
				int corona_ind = it->first;
				for (auto it1 = map_combinations_id_level[corona_ind].begin(); it1 != map_combinations_id_level[corona_ind].end(); it1++) {
					string comb = it1->first;
					int count = it1->second;
					vector<string> fields;
					split(fields, comb, boost::is_any_of("#"));
					fprintf(fout_comb, "%s,%i,", "id_level", corona_ind);
					for (int jj = 0; jj < fields.size(); jj++) {
						if (fields[jj] != "") fprintf(fout_comb, "%s,", fields[jj].c_str());
					}
					fprintf(fout_comb, "%i\n", count);
				}
			}

			fclose(fout_comb);
		}

		
		//***** hosp icu death ****/
		if (1 == 2) {
			int count_all = 0, count_icu = 0, count_death = 0, count_hosp = 0;
			
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				int age = id_age[id];
				int gender = id_gender[id];
				int corona_ind = id_corona_ind_new[id];
				if (corona_ind != 1) continue;

				count_all++;
				if (map_id_hosp.find(id) != map_id_hosp.end()) {
					count_hosp++;
					//fprintf(stderr, "%i \n", id);
					//getchar();
				}
				if (map_id_icu.find(id) != map_id_icu.end()) count_icu++;
				if (map_id_death1.find(id) != map_id_death1.end()) count_death++;

				
			}

			string fname_hist = "C:\\corona_segal\\hosp_icu_death_";
			if (qq == 0) {
				fname_hist = fname_hist + "quest.csv";
			}
			else if (qq == 1) {
				fname_hist = fname_hist + "doctor.csv";
			}
			else {
				fname_hist = fname_hist + "quest_doctor.csv";
			}

			FILE* fout = fopen(fname_hist.c_str(), "w");
			fprintf(fout, "%s,%s,%s,%s\n", "all", "hosp", "icu", "death");
			fprintf(fout, "%i,%i,%i,%i\n", count_all, count_hosp, count_icu, count_death);
			fclose(fout);
		}
		
		/***** age histogram *****/ 
		if (1 == 2) {
			map<int,map<int, map<int, int>>> map_id_gender_age;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				int age = id_age[id];
				int gender = id_gender[id];
				int corona_ind = id_corona_ind_new[id];
				map_id_gender_age[corona_ind][gender][age]++;
			}

			string fname_hist = "C:\\corona_segal\\gender_age_hist_";
			if (qq == 0) {
				fname_hist = fname_hist + "quest.csv";
			}
			else if (qq == 1) {
				fname_hist = fname_hist + "doctor.csv";
			}
			else {
				fname_hist = fname_hist + "quest_doctor.csv";
			}

			FILE* fout = fopen(fname_hist.c_str(), "w");
			fprintf(fout, "%s,%s,%s,%s\n", "corona_ind", "gender", "age", "count_id");
			for (auto it = map_id_gender_age.begin(); it != map_id_gender_age.end(); it++) {
				int corona_ind = it->first;
				for (auto it1 = map_id_gender_age[corona_ind].begin(); it1 != map_id_gender_age[corona_ind].end(); it1++) {
					int gender = it1->first;
					for (auto it2 = map_id_gender_age[corona_ind][gender].begin(); it2 != map_id_gender_age[corona_ind][gender].end(); it2++) {
						int age = it2->first;
						int count = it2->second;
						fprintf(fout, "%i,%i,%i,%i\n", corona_ind, gender, age, count);
					}
				}
			}
			fclose(fout);
		}

		//*** print ids
		if (1 == 2) {

			fprintf(stderr, "****** print id ...... ******** \n");
			map<int, map<int, vector<string>>> map_id_addtional_vec;
			if (1 == 1) {
				int count = 0;
				string fname = "quest_for_qa_sample_additional.txt";
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						count++;
						if (count % 100000 == 0) fprintf(stderr, "%i\n", count);
						if (count == 1) {
							split(fields, curr_line, boost::is_any_of("\t"));
							continue;
						}

						split(fields, curr_line, boost::is_any_of("\t"));
						int id = stoi(fields[1]);
						string milui_date = fields[0];
						int milui_date_n = convert_date_milui(milui_date);//stoi(milui_date.substr(6, 4) + milui_date.substr(3, 2) + milui_date.substr(0, 2));						
						string add1 = fields[2];
						string add2 = fields[3];
						map_id_addtional_vec[id][milui_date_n].push_back(add1);
						map_id_addtional_vec[id][milui_date_n].push_back(add2);
					}
				}
			}





			string fname_sample = symp_dir + "article_samples.txt";
			FILE* fout = fopen(fname_sample.c_str(), "w");
			fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
				"numerator", "date", "signal", "date_pos", "n_days", "n_days_before_test", "n_symp", "n_symp_uniq",
				"n_symp_uniq_good", "doctor_ind", "quest_ind", "diff_day_min_max","id_with_doctor","id_with_quest","id_with_doctor_quest");

			for (int mm = 5; mm <= 14; mm++)
				fprintf(fout, "%s\t", map_symp_names[mm].c_str());

			fprintf(fout, "%s\t%s\t", "reason_for_bidud", "other_symptom");

			fprintf(fout, "\n");

			for (auto it = id_n_dates.begin(); it != id_n_dates.end(); it++) {
				int id = it->first;
				int n_days = it->second;
				int n_symp = id_n_symp[id];
				int n_symp_uniq = (int)id_n_symp_uniq[id].size();
				int n_symp_uniq_good = (int)id_n_symp_uniq_good[id].size();
				int n_dates_before_test = id_n_dates_before_test[id];

				if (n_days < 2) continue;
				if (n_symp < 3) continue;

				//if (n_days < 3) continue;
				//if (n_symp < 4) continue;

				if (corona_pos_final.find(id) == corona_pos_final.end()) continue;

				vector<int> temp_dates = id_vec_dates[id];
				sort(temp_dates.begin(), temp_dates.end());
				//fprintf(stderr, "id %i size %i \n", id, (int)temp_dates.size());
				int day_min_max = datediff(temp_dates[0], temp_dates[(int)temp_dates.size() - 1]);


				int date_pos = corona_pos_final[id];
				int numerator = id;

				int id_with_doctor = 0;
				if (id_date_diags.find(id) != id_date_diags.end()) id_with_doctor = 1;
				int id_with_quest = 0;
				if (map_id_date_all_symp.find(id) != map_id_date_all_symp.end()) id_with_quest = 1;

				int id_with_doctor_quest = 0;
				if (id_with_doctor == 1 && id_with_quest == 1) id_with_doctor_quest = 1;

				for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
					int milui_date = it1->first;
					for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_join[numerator][milui_date][jj];

						int doctor_ind = 0;
						int quest_ind = 0;

						if (map_id_date_all_symp.find(numerator) != map_id_date_all_symp.end()) {
							if (map_id_date_all_symp[numerator].find(milui_date) != map_id_date_all_symp[numerator].end()) {
								for (int jj = 0; jj < map_id_date_all_symp[numerator][milui_date].size(); jj++) {
									string signal_qa = map_id_date_all_symp[numerator][milui_date][jj];
									if (signal == signal_qa) {
										quest_ind = 1;
										id_with_quest = 1;
									}
								}
							}
						}

						if (id_date_diags.find(numerator) != id_date_diags.end()) {
							if (id_date_diags[numerator].find(milui_date) != id_date_diags[numerator].end()) {
								for (int jj = 0; jj < id_date_diags[numerator][milui_date].size(); jj++) {
									string signal_qa = id_date_diags[numerator][milui_date][jj];
									if (signal == signal_qa) {
										doctor_ind = 1;
										id_with_doctor = 1;
									}
								}
							}
						}


						fprintf(fout, "%i\t%i\t%s\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t",
							numerator, milui_date, signal.c_str(), date_pos, n_days, n_dates_before_test, n_symp,
							n_symp_uniq, n_symp_uniq_good, doctor_ind, quest_ind, day_min_max, id_with_doctor, id_with_quest, id_with_doctor_quest);

						if (quest_ind == 1) {
							for (int mm = 0; mm < 10; mm++) {
								fprintf(fout, "%s\t", map_id_bidud_vec[numerator][milui_date][mm].c_str());
							}
							fprintf(fout, "%s\t", map_id_addtional_vec[numerator][milui_date][0].c_str());
							fprintf(fout, "%s\t", map_id_addtional_vec[numerator][milui_date][1].c_str());
						}
						else {
							for (int mm = 0; mm < 10; mm++) {
								fprintf(fout, "%s\t", "");
							}
							fprintf(fout, "%s\t", "");
							fprintf(fout, "%s\t", "");
						}

						fprintf(fout, "\n");

					}
				}
			
			
				for (auto it5 = id_all_tests[id].begin(); it5!=id_all_tests[id].end(); it5++) {
					int milui_date = it5->first;
					string corona_test = it5->second;
					//fprintf(fout, "%i\t%i\t%s\n", id, date, corona_test.c_str());
					fprintf(fout, "%i\t%i\t%s\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n",
						numerator, milui_date, corona_test.c_str(), date_pos, n_days, n_dates_before_test, n_symp,
						n_symp_uniq, n_symp_uniq_good, 0, 0, day_min_max, id_with_doctor, id_with_quest, id_with_doctor_quest);

				}

				
				if (map_id_recover.find(id) != map_id_recover.end()) {
					//fprintf(fout, "%i\t%i\t%s\n", id, map_id_recover[id], "patient recoverd");
					fprintf(fout, "%i\t%i\t%s\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n",
						numerator, map_id_recover[id], "patient recoverd", date_pos, n_days, n_dates_before_test, n_symp,
						n_symp_uniq, n_symp_uniq_good, 0, 0, day_min_max, id_with_doctor, id_with_quest, id_with_doctor_quest);

				}
			
			}
			fclose(fout);
		}

		//*****  moving avg around diagnosis date ******///
		if (1 == 2) {
			string moving_avg_dir="";
			string moving_avg_fname = "";

			if (RUN_ON_QUEST == 1) {
				moving_avg_dir = "C:\\corona_segal\\article_graph_nir_from_quest\\";
				moving_avg_fname = moving_avg_dir + "data_quest_time_analysis_around_diagnosys_date.txt";
			}

			if (RUN_ON_DOCTOR == 1) {
				moving_avg_dir = "C:\\corona_segal\\article_graph_nir_from_doctor_smadar\\";
				moving_avg_fname = moving_avg_dir + "data_doctor_time_analysis_around_diagnosys_date.txt";
			}


			fprintf(stderr, "segal anlysis moving avg ...\n");
			//corona pos / diff / signal / count

			FILE* fout = fopen(moving_avg_fname.c_str(), "w");
			fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\n", "win", "corona_ind", "diff_from_test", "param", "count_symptom", "count_all");
			//for (int mm = 1; mm <= 9; mm += 2) {
			for (int mm = 1; mm <= 9; mm += 2) {
				int curr_win = mm;

				map<string, map<int, map<int, int>>> map_pos_diff_signal_count;
				map<int, map<int, int>> map_pos_diff_count;

				for (int hh = -31; hh <= 31; hh++) {
					//for (int hh = -21; hh <= 21; hh++) {
					int curr_diff = hh;
					int curr_diff_from = curr_diff - ((curr_win - 1) / 2);
					int curr_diff_to = curr_diff + ((curr_win - 1) / 2);
					for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
						int numerator = it->first;
						//if (tzevet == 1) continue;

						int corona_test_ind = -9;
						int corona_test_date = -9;
						{
							if (corona_pos_final.find(numerator) != corona_pos_final.end() && test_pos_first.find(numerator)!= test_pos_first.end()) {
								corona_test_ind = 1;
								corona_test_date = corona_pos_final[numerator];
							}
							else if (corona_pos_final.find(numerator) != corona_pos_final.end()) {
								corona_test_ind = 3;
								corona_test_date = test_neg_first[numerator];
							}
							else if (test_neg_first.find(numerator) != test_neg_first.end()) {
								corona_test_ind = 0;
								corona_test_date = test_neg_first[numerator];
							}
							else {
								corona_test_ind = 2;
								int idx = rand() % n_vec_neg_dates;
								corona_test_date = vec_neg_dates[idx];
							}
						}

						//if (corona_test_ind == 2) corona_test_ind = 3;
						//if (corona_test_ind == 1 && id_gender[numerator] == 1) corona_test_ind = 2;


						int milui_in = 0;
						map<string, int> signal_in;
						map<string, int> signal_hist;
						for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
							int milui_date = it1->first;
							int milui_diff = datediff(corona_test_date, milui_date);
							//fprintf(stderr, "corona_test_date %i milui_date %i  milui_diff %i curr_diff_from %i curr_diff_to %i  \n", corona_test_date, milui_date, milui_diff, curr_diff_from, curr_diff_to);
							//getchar();

							if (milui_diff >= curr_diff_from && milui_diff <= curr_diff_to) {
								milui_in = 1;
								//getchar();
								for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
									string signal = map_id_date_join[numerator][milui_date][jj];
									//taking only first time ..
									//if (signal_hist.find(signal) == signal_hist.end()) {
									signal_in[signal] = 1;
									//}
									//else {
										//fprintf(stderr, "%i %i %s \n", numerator, milui_date, signal.c_str());
										//getchar();
									//}
								}
							}

							for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
								string signal = map_id_date_join[numerator][milui_date][jj];
								signal_hist[signal] = 1;
							}

						}



						map_pos_diff_count[corona_test_ind][curr_diff] += milui_in;
						for (auto it1 = signal_in.begin(); it1 != signal_in.end(); it1++) {
							string signal = it1->first;
							map_pos_diff_signal_count[signal][corona_test_ind][curr_diff]++;
						}
					}
				}

				if (1 == 1) {

					for (auto it = map_pos_diff_signal_count.begin(); it != map_pos_diff_signal_count.end(); it++) {
						string symp = it->first;
						//symp = "symp_keev_rosh";
						for (auto it1 = map_pos_diff_signal_count[symp].begin(); it1 != map_pos_diff_signal_count[symp].end(); it1++) {
							int corona_ind = it1->first;
							for (auto it2 = map_pos_diff_signal_count[symp][corona_ind].begin(); it2 != map_pos_diff_signal_count[symp][corona_ind].end(); it2++) {
								int diff_to_test = it2->first;
								int count = it2->second;
								int count_all = map_pos_diff_count[corona_ind][diff_to_test];
								fprintf(fout, "%i\t%i\t%i\t%s\t%i\t%i\n", mm, corona_ind, diff_to_test, symp.c_str(), count, count_all);
								fprintf(stderr, "%i\t%i\t%i\t%s\t%i\t%i\n", mm, corona_ind, diff_to_test, symp.c_str(), count, count_all);
							}
						}
						//break;
					}
				}


				for (auto it = map_pos_diff_signal_count.begin(); it != map_pos_diff_signal_count.end(); it++) {
					string symp = it->first;
					//symp = "symp_keev_rosh";
					string plot_name = symp + " win " + to_string(curr_win) + " days";
					string plot_name1 = "Time_analysis";
					plot_bar_segal_symp(plot_name, map_pos_diff_signal_count[symp], map_pos_diff_count, plot_name1, moving_avg_dir);
					//break;
				}
			}
			fclose(fout);
			//corona pos / diff / signal / count
			//map<int, map<int, map<string, int>>> map_pos_diff_signal_count;

		}

		/*** couple analysis ****/
		if (1 == 2) {
			string fname_couple = symp_dir + "article_couple_analysis.txt";
			FILE* fout_couple = fopen(fname_couple.c_str(), "w");

			for (int gg = 0; gg < 2; gg++) {
				map<string, int> symp_all;
				map<int, map<int, int>> map_diff_no_symp_symp;
				//map<int, int> map_diff_any_symp;
				map<string, map<string, int>> symp_couple;
				for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
					int id = it->first;
					if (gg == 0) {
						if (corona_pos_final.find(id) == corona_pos_final.end()) continue;
					}
					else {
						if (corona_pos_final.find(id) != corona_pos_final.end()) continue;
						if (test_neg_first.find(id) == test_neg_first.end()) continue;
					}

					map<string, int> symp_hist;

					for (auto it1 = map_id_date_join[id].begin(); it1 != map_id_date_join[id].end(); it1++) {
						int date = it1->first;
						int qa_diff;
						if (gg == 0)
							qa_diff = datediff(corona_pos_final[id], date);
						else
							qa_diff = datediff(test_neg_first[id], date);

						//if (qa_diff >= -21 && qa_diff <= 21) {
						//cehck couple
						int find = 0;
						for (int jj = 0; jj < it1->second.size(); jj++) {
							string symp = it1->second[jj];
							//if (symp == "chom_up_to_374" || symp == "symp_none" || symp == "symp_other") continue;
							if (symp == "chom_up_to_374" || symp == "symp_none") continue;
							if (symp_hist.find(symp) != symp_hist.end()) continue;  //take only first
							if (find == 0) {
								map_diff_no_symp_symp[1][qa_diff]++;
								find = 1;
							}
							symp_all[symp]++;
							for (auto it2 = symp_hist.begin(); it2 != symp_hist.end(); it2++) {
								string symp_prev = it2->first;
								if (symp_prev != symp) {
									symp_couple[symp_prev][symp]++;
									//fprintf(stderr, "id %i date %i symp_prev %s symp %s \n", id, date, symp_prev.c_str(), symp.c_str());
									//getchar();
								}
							}
						}

						if (find == 0) {
							map_diff_no_symp_symp[0][qa_diff]++;
							//find = 1;
						}

						//enter to hist
						for (int jj = 0; jj < it1->second.size(); jj++) {
							string symp = it1->second[jj];
							//if (symp == "chom_up_to_374" || symp == "symp_none" || symp == "symp_other") continue;
							if (symp == "chom_up_to_374" || symp == "symp_none") continue;
							symp_hist[symp] = 1;
						}
						//}
					}
				}

				map<string, int> map_already;
				for (auto it = symp_all.begin(); it != symp_all.end(); it++) {
					string symp1 = it->first;
					for (auto it1 = symp_all.begin(); it1 != symp_all.end(); it1++) {
						string symp2 = it1->first;

						string qa = symp1 + symp2;
						if (map_already.find(qa) != map_already.end()) continue;

						int count_before = 0;
						int count_after = 0;
						if (symp_couple.find(symp1) != symp_couple.end()) {
							if (symp_couple[symp1].find(symp2) != symp_couple[symp1].end()) {
								count_before += symp_couple[symp1][symp2];
							}
						}

						if (symp_couple.find(symp2) != symp_couple.end()) {
							if (symp_couple[symp2].find(symp1) != symp_couple[symp2].end()) {
								count_after += symp_couple[symp2][symp1];
							}
						}

						if ((count_before + count_after) == 0) continue;

						string corona_test_s = "pos";
						if (gg == 1)corona_test_s = "neg";

						fprintf(fout_couple, "%s\t%s\t%s\t%i\t%i\t%i\t%f\n", corona_test_s.c_str(), symp1.c_str(), symp2.c_str(), count_before, count_after, count_before + count_after, (float)count_before / ((float)count_before + (float)count_after));

						string cp1 = symp1 + symp2;
						//string cp2 = symp2 + symp1;
						map_already[cp1] = 1;
						//map_already[cp2] = 1;
					}
				}



			}

			fclose(fout_couple);
		}


		/**** hosp analisys ****/
		map<int, int> id_hosp;
		map<int, int> is_severe_hosp;
		if (1 == 2) {
			//read hosp;
			int count = 0;
			string fname = hosp_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					id_hosp[id] = 1;
				}
			}

			//icu
			if (1 == 1) {
				string fname = symp_dir + "niftar.txt";
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						is_severe_hosp[stoi(curr_line)] = 1;
					}
				}
			}
			if (1 == 1) {
				string fname = symp_dir + "kashe.txt";
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						is_severe_hosp[stoi(curr_line)] = 1;
					}
				}
			}
			if (1 == 1) {
				string fname = symp_dir + "beinoni.txt";
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						is_severe_hosp[stoi(curr_line)] = 1;
					}
				}
			}
			if (1 == 1) {
				string fname = symp_dir + "icu.txt";
				ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
				while (getline(inf, curr_line)) {
					if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						is_severe_hosp[stoi(curr_line)] = 1;
					}
				}
			}

			for (auto it = is_severe_hosp.begin(); it != is_severe_hosp.end(); it++) {
				int id = it->first;
				id_hosp[id] = 1;
			}

			int count_pos_all = 0;
			int count_pos_in_hosp = 0;
			int count_pos_in_hosp_severe = 0;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				if (corona_pos_final.find(id) == corona_pos_final.end()) continue;

				count_pos_all++;
				if (id_hosp.find(id) != id_hosp.end()) count_pos_in_hosp++;
				if (is_severe_hosp.find(id) != is_severe_hosp.end()) count_pos_in_hosp_severe++;
			}


			fprintf(stderr, "count_pos_all  %i \n", count_pos_all);
			fprintf(stderr, "count_pos_in_hosp  %i \n", count_pos_in_hosp);
			fprintf(stderr, "count_pos_in_hosp_severe  %i \n", count_pos_in_hosp_severe);

		}


		/**** time of symptom anaysis ******/
		if (1 == 2) {
			fprintf(stderr, "symp diff analysis .....\n");
			map<string, int> map_symp_from_quest;
			for (auto it = map_id_date_all_symp.begin(); it != map_id_date_all_symp.end(); it++) {
				int numerator = it->first;
				for (auto it1 = map_id_date_all_symp[numerator].begin(); it1 != map_id_date_all_symp[numerator].end(); it1++) {
					int milui_date = it1->first;
					for (int jj = 0; jj < map_id_date_all_symp[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_all_symp[numerator][milui_date][jj];
						if (signal != "symp_none" && signal != "chom_up_to_374")
							map_symp_from_quest[signal] = 1;
					}
				}
			}
			map_symp_from_quest["symp_fever"] = 1;

			map<string, vector<float>> map_symp_time;
			map<string, vector<float>> map_symp_time_max;

			for (auto it = id_n_dates.begin(); it != id_n_dates.end(); it++) {
				int numerator = it->first;
				//if (numerator != 78988) continue;

				if (corona_pos_final.find(numerator) == corona_pos_final.end()) continue;

				map<string, int> symp_min_date;
				map<string, int> symp_max_date;
				vector <int> milui_date_vec;
				for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
					int milui_date = it1->first;
					milui_date_vec.push_back(milui_date);
					for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_join[numerator][milui_date][jj];
						//take symptos only from quest
						if (map_symp_from_quest.find(signal) == map_symp_from_quest.end()) continue;
						if (symp_min_date.find(signal) == symp_min_date.end()) symp_min_date[signal] = milui_date;
						symp_max_date[signal] = milui_date;
					}
				}

				if ((int)milui_date_vec.size() <= 1) continue;

				//for (int kk = 0; kk < milui_date_vec.size(); kk++) {
					//fprintf(stderr, "=== date vec %i \n", milui_date_vec[kk]);
				//}

				for (auto it2 = symp_min_date.begin(); it2 != symp_min_date.end(); it2++) {
					string signal = it2->first;
					if (symp_min_date[signal] != symp_max_date[signal]) {
						int diff = datediff(symp_min_date[signal], symp_max_date[signal]) + 1;
						map_symp_time[signal].push_back((float)diff);
						//fprintf(stderr, "id %i diff %i signal %s ", numerator, diff, signal.c_str() );
						//getchar();
					}

					int date_before = symp_min_date[signal];
					int date_before_orig = symp_min_date[signal];
					for (int kk = 0; kk < milui_date_vec.size(); kk++) {
						//fprintf(stderr, "before try %i \n", milui_date_vec[kk]);
						if (milui_date_vec[kk] >= symp_min_date[signal]) break;
						date_before = milui_date_vec[kk];
					}


					int date_after = symp_max_date[signal];
					int date_after_orig = symp_max_date[signal];
					for (int kk = (int)milui_date_vec.size() - 1; kk >= 0; kk--) {
						//fprintf(stderr, "after try %i \n", milui_date_vec[kk]);
						if (milui_date_vec[kk] <= symp_max_date[signal]) break;
						date_after = milui_date_vec[kk];
					}

					if (date_after != date_after_orig) date_after = dateadd(date_after, -1);
					if (date_before != date_before_orig) date_before = dateadd(date_before, 1);
					int max_diff = datediff(date_before, date_after) + 1;
					map_symp_time_max[signal].push_back(max_diff);
					//fprintf(stderr, "id %i max_diff %i signal %s min_date %i max_date %i ", numerator, max_diff, signal.c_str(), date_before, date_after);
					//getchar();
				}

			}

			fprintf(stderr, "==== min diff =====\n");
			for (auto it = map_symp_time.begin(); it != map_symp_time.end(); it++) {
				string symp = it->first;
				int n = (int)it->second.size();
				float symp_diff_min = get_min(it->second);
				float symp_diff_max = get_max(it->second);
				float symp_diff_mean = get_mean(it->second);
				float symp_diff_sdv = get_sdv(it->second);
				fprintf(stderr, "%s %i %f %f %f %f \n", symp.c_str(), n, symp_diff_mean, symp_diff_sdv, symp_diff_min, symp_diff_max);
			}

			fprintf(stderr, "==== max diff =====\n");
			for (auto it = map_symp_time_max.begin(); it != map_symp_time_max.end(); it++) {
				string symp = it->first;
				int n = (int)it->second.size();
				float symp_diff_min = get_min(it->second);
				float symp_diff_max = get_max(it->second);
				float symp_diff_mean = get_mean(it->second);
				float symp_diff_sdv = get_sdv(it->second);
				fprintf(stderr, "%s %i %f %f %f %f \n", symp.c_str(), n, symp_diff_mean, symp_diff_sdv, symp_diff_min, symp_diff_max);
			}

		}

		/**** time of symptom anaysis new ******/
		if (1 == 2) {
			fprintf(stderr, "symp diff analysis .....\n");
			map<string, int> map_symp_from_quest;
			for (auto it = map_id_date_all_symp.begin(); it != map_id_date_all_symp.end(); it++) {
				int numerator = it->first;
				for (auto it1 = map_id_date_all_symp[numerator].begin(); it1 != map_id_date_all_symp[numerator].end(); it1++) {
					int milui_date = it1->first;
					for (int jj = 0; jj < map_id_date_all_symp[numerator][milui_date].size(); jj++) {
						string signal = map_id_date_all_symp[numerator][milui_date][jj];
						if (signal != "symp_none" && signal != "chom_up_to_374")
							map_symp_from_quest[signal] = 1;
					}
				}
			}
			map_symp_from_quest["symp_fever"] = 1;



			//string fname_min = symp_dir + "data_quest_min_time_of_symptom.csv";
			//FILE *fout_time = fopen(fname_min.c_str(), "w");


			string fname_max = symp_dir + "data_quest_min_max_time_of_symptom.csv";
			FILE* fout_time_max = fopen(fname_max.c_str(), "w");
			fprintf(fout_time_max, "%s,%s,%s,%s,%s\n", "min_max", "symp", "corona_ind", "diff_time", "count");

			for (int mm = 0; mm < 1; mm++) {
				int corona_ind = mm;

				map<string, vector<float>> map_symp_time;
				map<string, vector<float>> map_symp_time_max;

				for (auto it = id_n_dates.begin(); it != id_n_dates.end(); it++) {
					int numerator = it->first;
					//if (numerator != 78988) continue;

					if (corona_ind == 1) {
						if (corona_pos_final.find(numerator) == corona_pos_final.end()) continue;
					}
					else if (corona_ind == 0) {
						if (corona_pos_final.find(numerator) != corona_pos_final.end()) continue;
						if (test_neg_first.find(numerator) == test_neg_first.end()) continue;
					}
					else if (corona_ind == 2) {
						if (corona_pos_final.find(numerator) != corona_pos_final.end()) continue;
						if (test_neg_first.find(numerator) != test_neg_first.end()) continue;
					}

					map<string, int> symp_min_date;
					map<string, int> symp_max_date;
					vector <int> milui_date_vec;
					for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
						int milui_date = it1->first;
						milui_date_vec.push_back(milui_date);
						for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
							string signal = map_id_date_join[numerator][milui_date][jj];
							//take symptos only from quest
							if (map_symp_from_quest.find(signal) == map_symp_from_quest.end()) continue;
							if (symp_min_date.find(signal) == symp_min_date.end()) symp_min_date[signal] = milui_date;
							symp_max_date[signal] = milui_date;
						}
					}

					if ((int)milui_date_vec.size() <= 1) continue;

					//for (int kk = 0; kk < milui_date_vec.size(); kk++) {
						//fprintf(stderr, "=== date vec %i \n", milui_date_vec[kk]);
					//}

					for (auto it2 = symp_min_date.begin(); it2 != symp_min_date.end(); it2++) {
						string signal = it2->first;

						//if (signal != "symp_ayefut") continue;

						if (symp_min_date[signal] != symp_max_date[signal]) {
							int diff = datediff(symp_min_date[signal], symp_max_date[signal]) + 1;
							map_symp_time[signal].push_back((float)diff);
							//fprintf(stderr, "min_diff === : id %i diff %i signal %s ", numerator, diff, signal.c_str());
							//getchar();
						}

						int date_before = symp_min_date[signal];
						int date_before_orig = symp_min_date[signal];
						for (int kk = 0; kk < milui_date_vec.size(); kk++) {
							//fprintf(stderr, "before try %i \n", milui_date_vec[kk]);
							if (milui_date_vec[kk] >= symp_min_date[signal]) break;
							date_before = milui_date_vec[kk];
						}


						int date_after = symp_max_date[signal];
						int date_after_orig = symp_max_date[signal];
						for (int kk = (int)milui_date_vec.size() - 1; kk >= 0; kk--) {
							//fprintf(stderr, "after try %i \n", milui_date_vec[kk]);
							if (milui_date_vec[kk] <= symp_max_date[signal]) break;
							date_after = milui_date_vec[kk];
						}

						if (date_after != date_after_orig) date_after = dateadd(date_after, -1);
						if (date_before != date_before_orig) date_before = dateadd(date_before, 1);
						int max_diff = datediff(date_before, date_after) + 1;
						map_symp_time_max[signal].push_back(max_diff);

						//fprintf(stderr, "max_diff ==== id %i max_diff %i signal %s min_date %i max_date %i ", numerator, max_diff, signal.c_str(), date_before, date_after);
						//getchar();
					}

				}

				fprintf(stderr, "==== min diff =====\n");
				for (auto it = map_symp_time.begin(); it != map_symp_time.end(); it++) {
					string symp = it->first;
					int n = (int)it->second.size();
					float symp_diff_min = get_min(it->second);
					float symp_diff_max = get_max(it->second);
					float symp_diff_mean = get_mean(it->second);
					float symp_diff_sdv = get_sdv(it->second);
					fprintf(stderr, "%s %i %f %f %f %f \n", symp.c_str(), n, symp_diff_mean, symp_diff_sdv, symp_diff_min, symp_diff_max);
					//getchar();
					map<int, int> temp_map_count;
					for (int ii = 0; ii < it->second.size(); ii++) {
						int val = it->second[ii];
						temp_map_count[val]++;
					}
					for (auto it3 = temp_map_count.begin(); it3 != temp_map_count.end(); it3++) {
						fprintf(fout_time_max, "%s,%s,%i,%i,%i\n", "min", symp.c_str(), corona_ind, it3->first, it3->second);
					}
				}

				fprintf(stderr, "==== max diff =====\n");
				for (auto it = map_symp_time_max.begin(); it != map_symp_time_max.end(); it++) {
					string symp = it->first;
					int n = (int)it->second.size();
					float symp_diff_min = get_min(it->second);
					float symp_diff_max = get_max(it->second);
					float symp_diff_mean = get_mean(it->second);
					float symp_diff_sdv = get_sdv(it->second);
					fprintf(stderr, "%s %i %f %f %f %f \n", symp.c_str(), n, symp_diff_mean, symp_diff_sdv, symp_diff_min, symp_diff_max);


					map<int, int> temp_map_count;
					for (int ii = 0; ii < it->second.size(); ii++) {
						int val = it->second[ii];
						temp_map_count[val]++;
					}

					for (auto it3 = temp_map_count.begin(); it3 != temp_map_count.end(); it3++) {
						fprintf(fout_time_max, "%s,%s,%i,%i,%i\n", "max", symp.c_str(), corona_ind, it3->first, it3->second);
					}

				}
			}

		}

		//*****  moving avg around recover date ******///
		if (1 == 2) {
			//*** moving avg
			
			string moving_avg_dir="";
			string moving_avg_fname = "";

			if (RUN_ON_QUEST == 1) {
				moving_avg_dir = "C:\\corona_segal\\article_graph_nir_from_quest_recover\\";
				moving_avg_fname = moving_avg_dir + "data_quest_time_analysis_around_recover_date.txt";
			}

			if (RUN_ON_DOCTOR == 1) {
				moving_avg_dir = "C:\\corona_segal\\article_graph_nir_from_doctor_recover_smadar\\";
				moving_avg_fname = moving_avg_dir + "data_doctor_time_analysis_around_recover_date.txt";
			}


			if (1 == 1) {
				fprintf(stderr, "segal anlysis 1 ...\n");
				//corona pos / diff / signal / count

				FILE* fout = fopen(moving_avg_fname.c_str(), "w");
				fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\n", "win", "corona_ind", "diff_from_test", "param", "count_symptom", "count_all");
				for (int mm = 1; mm <= 9; mm += 2) {
					//for (int mm = 5; mm <= 5; mm += 2) {
					int curr_win = mm;

					map<string, map<int, map<int, int>>> map_pos_diff_signal_count;
					map<int, map<int, int>> map_pos_diff_count;

					//for (int hh = -14; hh <= 14; hh++) {
					for (int hh = -51; hh <= 31; hh++) {
						int curr_diff = hh;
						int curr_diff_from = curr_diff - ((curr_win - 1) / 2);
						int curr_diff_to = curr_diff + ((curr_win - 1) / 2);
						for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
							int numerator = it->first;
							//if (tzevet == 1) continue;

							int corona_test_ind = -9;
							int corona_test_date = -9;
							{
								if (corona_pos_final.find(numerator) != corona_pos_final.end()) {
									corona_test_ind = 1;
									corona_test_date = corona_pos_final[numerator];
								}
								else if (test_neg_first.find(numerator) != test_neg_first.end()) {
									corona_test_ind = 0;
									corona_test_date = test_neg_first[numerator];
								}
								else {
									corona_test_ind = 2;
									int idx = rand() % n_vec_neg_dates;
									corona_test_date = vec_neg_dates[idx];
								}
							}

							if (corona_test_ind != 1) continue;
							if (map_id_recover.find(numerator) == map_id_recover.end()) continue;
							corona_test_date = map_id_recover[numerator];
							//fprintf(stderr, "%i %i \n", numerator, corona_test_date);
							//getchar();

							int milui_in = 0;
							map<string, int> signal_in;
							map<string, int> signal_hist;
							for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
								int milui_date = it1->first;
								int milui_diff = datediff(corona_test_date, milui_date);
								//fprintf(stderr, "corona_test_date %i milui_date %i  milui_diff %i curr_diff_from %i curr_diff_to %i  \n", corona_test_date, milui_date, milui_diff, curr_diff_from, curr_diff_to);
								//getchar();
								if (milui_diff >= curr_diff_from && milui_diff <= curr_diff_to) {
									milui_in = 1;
									//getchar();
									for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
										string signal = map_id_date_join[numerator][milui_date][jj];
										//taking only first time ..
										//if (signal_hist.find(signal) == signal_hist.end()) {
										signal_in[signal] = 1;
										//}
										//else {
										//fprintf(stderr, "%i %i %s \n", numerator, milui_date, signal.c_str());
										//getchar();
										//}
									}
								}

								for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
									string signal = map_id_date_join[numerator][milui_date][jj];
									signal_hist[signal] = 1;
								}

							}



							map_pos_diff_count[corona_test_ind][curr_diff] += milui_in;
							for (auto it1 = signal_in.begin(); it1 != signal_in.end(); it1++) {
								string signal = it1->first;
								map_pos_diff_signal_count[signal][corona_test_ind][curr_diff]++;
							}
						}
					}

					if (1 == 1) {

						for (auto it = map_pos_diff_signal_count.begin(); it != map_pos_diff_signal_count.end(); it++) {
							string symp = it->first;
							//symp = "symp_keev_rosh";
							for (auto it1 = map_pos_diff_signal_count[symp].begin(); it1 != map_pos_diff_signal_count[symp].end(); it1++) {
								int corona_ind = it1->first;
								for (auto it2 = map_pos_diff_signal_count[symp][corona_ind].begin(); it2 != map_pos_diff_signal_count[symp][corona_ind].end(); it2++) {
									int diff_to_test = it2->first;
									int count = it2->second;
									int count_all = map_pos_diff_count[corona_ind][diff_to_test];
									fprintf(fout, "%i\t%i\t%i\t%s\t%i\t%i\n", mm, corona_ind, diff_to_test, symp.c_str(), count, count_all);
									fprintf(stderr, "%i\t%i\t%i\t%s\t%i\t%i\n", mm, corona_ind, diff_to_test, symp.c_str(), count, count_all);
								}
							}
							//break;
						}
					}


					for (auto it = map_pos_diff_signal_count.begin(); it != map_pos_diff_signal_count.end(); it++) {
						string symp = it->first;
						//symp = "symp_keev_rosh";
						string plot_name = symp + " win " + to_string(curr_win) + " days";
						string plot_name1 = "Time_analysis";
						plot_bar_segal_symp(plot_name, map_pos_diff_signal_count[symp], map_pos_diff_count, plot_name1, moving_avg_dir);
						//break;
					}
				}
				fclose(fout);
				//corona pos / diff / signal / count
				//map<int, map<int, map<string, int>>> map_pos_diff_signal_count;

			}



		}

		//time from pos to recover
		if (1 == 2) {
			//read-pop_file 
			string recover_time_fname = "";
			if (qq==0) recover_time_fname = "recover_time_from_pos_quest.csv";
			else if (qq == 1) recover_time_fname = "recover_time_from_pos_doctor.csv";
			else recover_time_fname = "recover_time_from_pos_doctor_quest.csv";

			FILE* fout = fopen(recover_time_fname.c_str(), "w");
			map<int, int> map_time_to_rec;

			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				if (test_pos_first.find(id) == test_pos_first.end()) continue;
				int pos_date = test_pos_first[id];
				if (map_id_recover.find(id) != map_id_recover.end()) {
					int rec_date = map_id_recover[id];
					int diff_to_rec = datediff(pos_date, rec_date);
					map_time_to_rec[diff_to_rec]++;
				}
				else {
					map_time_to_rec[-1]++;
				}
			}

			fprintf(fout, "time_from_pos_to_recover,count_id\n");
			for (auto it = map_time_to_rec.begin(); it != map_time_to_rec.end(); it++) {
				fprintf(fout, "%i,%i\n", it->first, it->second);
			}

			fclose(fout);

		}


		//time from pos to recover 2
		if (1 == 2) {
			//read-pop_file 
			vector<float> rec_all;
			vector<float> rec_male;
			vector<float> rec_female;
			vector<float> rec_0_17;
			vector<float> rec_18_100;

			int count_pos_table_1 = 0;
			int count_pos_table_1_with_rec = 0;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				if (test_pos_first.find(id) == test_pos_first.end()) continue;
				count_pos_table_1++;
				int pos_date = test_pos_first[id];
				if (map_id_recover.find(id) != map_id_recover.end()) {
					count_pos_table_1_with_rec++;
					int rec_date = map_id_recover[id];
					int diff_to_rec = datediff(pos_date, rec_date);
					
					rec_all.push_back((float)diff_to_rec);
					if (id_gender[id]==0) rec_male.push_back((float)diff_to_rec);
					if (id_gender[id] ==1) rec_female.push_back((float)diff_to_rec);
					if (id_age[id]<18) rec_0_17.push_back((float)diff_to_rec);
					if (id_age[id]>=18) rec_18_100.push_back((float)diff_to_rec);
				}
			}


			fprintf(stderr, "count_pos_table_1  %i \n", count_pos_table_1);
			fprintf(stderr, "count_pos_table_1_with_rec  %i \n", count_pos_table_1_with_rec);
			return 1;

			fprintf(stderr, "all %i %f %f \n", (int)rec_all.size(), get_mean(rec_all), get_sdv(rec_all));
			fprintf(stderr, "male %i %f %f \n", (int)rec_male.size(), get_mean(rec_male), get_sdv(rec_male));
			fprintf(stderr, "female %i %f %f \n", (int)rec_female.size(), get_mean(rec_female), get_sdv(rec_female));
			fprintf(stderr, "age_0_17 %i %f %f \n", (int)rec_0_17.size(), get_mean(rec_0_17), get_sdv(rec_0_17));
			fprintf(stderr, "age_18_100 %i %f %f \n", (int)rec_18_100.size(), get_mean(rec_18_100), get_sdv(rec_18_100));

			FILE* fout1 = fopen("recover_rec_male.txt", "w"); for (int ii = 0; ii <rec_male.size(); ii++) fprintf(fout1, "%f\n", rec_male[ii]);
			FILE* fout2 = fopen("recover_rec_female.txt", "w"); for (int ii = 0; ii < rec_female.size(); ii++) fprintf(fout2, "%f\n", rec_female[ii]);
			FILE* fout3 = fopen("recover_rec_0_17.txt", "w"); for (int ii = 0; ii < rec_0_17.size(); ii++) fprintf(fout3, "%f\n", rec_0_17[ii]);
			FILE* fout4 = fopen("recover_rec_18_100.txt", "w"); for (int ii = 0; ii < rec_18_100.size(); ii++) fprintf(fout4, "%f\n", rec_18_100[ii]);


		}

		//time from pos to recover by symptom
		if (1 == 2) {
			//read-pop_file 

			for (int mm = 0; mm < 1; mm++) {
				string before_string;
				
				if (mm==0) before_string = "_before";
				else before_string = "_all";

				fprintf(stderr, "**** %s *****\n", before_string.c_str());

				string recover_time_fname = "";
				if (RUN_ON_DOCTOR == 1) recover_time_fname = "recover_time_from_pos_doctor_symptom"+ before_string+".csv";
				else recover_time_fname = "recover_time_from_pos_quest_symptom" + before_string + ".csv";


				string recover_time_symp_fname = "";
				if (RUN_ON_DOCTOR == 1) recover_time_symp_fname = "recover_time_from_pos_doctor_symptom_avg" + before_string + ".csv";
				else recover_time_symp_fname = "recover_time_from_pos_quest_symptom_avg" + before_string + ".csv";


				FILE* fout1 = fopen(recover_time_symp_fname.c_str(), "w");
				fprintf(fout1, "signal,n,avg,sdv\n");

				FILE* fout = fopen(recover_time_fname.c_str(), "w");
				fprintf(fout, "signal,time_from_pos_to_recover,count_id\n");

				map<string, map<int, int>> map_sig_all;

				for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
					int numerator = it->first;
					if (test_pos_first.find(numerator) == test_pos_first.end()) continue;
					for (auto it1 = map_id_date_join[numerator].begin(); it1 != map_id_date_join[numerator].end(); it1++) {
						int milui_date = it1->first;
						if (mm == 0 && (milui_date > test_pos_first[numerator] || datediff(milui_date, test_pos_first[numerator])>21)   ) continue;
						for (int jj = 0; jj < map_id_date_join[numerator][milui_date].size(); jj++) {
							string signal = map_id_date_join[numerator][milui_date][jj];
								map_sig_all[signal][numerator] = 1;
						}
					}
				}


				

				for (auto it3 = map_sig_all.begin(); it3 != map_sig_all.end(); it3++) {
					string signal = it3->first;
					//fprintf(stderr, "%s\n", signal.c_str());
					map<int, int> map_time_to_rec;
					vector<float> vec_diff;
					for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
						int id = it->first;
						
						if (test_pos_first.find(id) == test_pos_first.end()) continue;
						if (map_sig_all[signal].find(id) == map_sig_all[signal].end())  continue;

						int pos_date = test_pos_first[id];
						if (map_id_recover.find(id) != map_id_recover.end()) {
							//fprintf(stderr, "sig %s id %i pos_date %i recover_date %i \n", signal.c_str(),  id , pos_date, map_id_recover[id]);
							//getchar();
							int rec_date = map_id_recover[id];
							int diff_to_rec = datediff(pos_date, rec_date);
							map_time_to_rec[diff_to_rec]++;
							vec_diff.push_back((float)diff_to_rec);
						}
						else {
							//map_time_to_rec[-1]++;
						}
					}

					float temp_mean = get_mean(vec_diff);
					float temp_sdv = get_sdv(vec_diff);

					fprintf(fout1, "%s,%i,%f,%f\n", signal.c_str(),(int)vec_diff.size(),  temp_mean, temp_sdv);
					
					for (auto it = map_time_to_rec.begin(); it != map_time_to_rec.end(); it++) {
						fprintf(fout, "%s,%i,%i\n", signal.c_str(),  it->first, it->second);
					}
				}

				fclose(fout);
				fclose(fout1);
			}

		}

		//time to recover by age 
		if (1 == 2) {
			map<string, vector<float>> vec_age_rec;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				if (id_age.find(id) == id_age.end()) {
					fprintf(stderr, "cant find age for id %i \n", id);
					getchar();
				}
				int age = id_age[id];
				string age_bin = "";
				if (age < 18) age_bin = "age_0_17";
				else if (age < 40) age_bin = "age_18_39";
				else if (age < 60) age_bin = "age_40_59";
				else if (age < 80) age_bin = "age_60_79";
				else age_bin = "age_80_100";

				if (test_pos_first.find(id) == test_pos_first.end()) continue;
				int pos_date = test_pos_first[id];
				if (map_id_recover.find(id) != map_id_recover.end()) {
					int rec_date = map_id_recover[id];
					int diff_to_rec = datediff(pos_date, rec_date);
					vec_age_rec[age_bin].push_back((float)diff_to_rec);
				}
			}

			for (auto it = vec_age_rec.begin(); it != vec_age_rec.end(); it++) {
				string age_bin = it->first;
				float age_mean = get_mean(it->second);
				float age_sdv = get_sdv(it->second);
				fprintf(stderr, "%s n : %i mean: %.2f sdv: %.2f  \n", age_bin.c_str(), (int)it->second.size(), age_mean, age_sdv);
			}


		}

		//recover to follow up
		//time from pos to recover
		if (1 == 2) {

			map<int, int> map_time_from_rec;
			vector<float> vec_diff;
			int count_diff_all = 0;
			int count_diff_after = 0;
			for (auto it = map_id_date_join.begin(); it != map_id_date_join.end(); it++) {
				int id = it->first;
				if (test_pos_first.find(id) == test_pos_first.end()) continue;
				int pos_date = test_pos_first[id];
				if (map_id_recover.find(id) != map_id_recover.end()) {
					count_diff_all++;
					int rec_date = map_id_recover[id];
					int last_date_diff = -1;
					for (auto it1 = map_id_date_join[id].rbegin(); it1 != map_id_date_join[id].rend(); it1++) {
						int last_date1 = it1->first;
						if (last_date1 > rec_date) {
							last_date_diff = datediff(rec_date, last_date1);
							count_diff_after++;
							vec_diff.push_back((float) last_date_diff);
							fprintf(stderr, "id %i rec_date  %i last_date1  %i ",id ,  rec_date, last_date1);
							getchar();
						}
						break;
					}
				}
			}
			fprintf(stderr, "recover_all = %i \n", count_diff_all);
			fprintf(stderr, "count_diff_after = %i \n", count_diff_after);
			fprintf(stderr, "mean : %f \n", get_mean(vec_diff));
			fprintf(stderr, "sdv : %f \n", get_sdv(vec_diff));



		}



		}

	}


	if (CORONA_ARTICLE_DOCTOR_MACCABI == 1) {

		string symp_dir = "C:\\corona_segal\\";
		string symp_fname = symp_dir + "corona_quest.txt";
		string tests = symp_dir + "tests.txt";
		string diag_fname = symp_dir + "Diagnosis.txt";
		string symp_id_fname = symp_dir + "ALL_DIAGNOSIS_FROM.txt";  //row data
		string symp_desc_fname = symp_dir + "diag_code_desc_from_esma.txt";  //row data
		string smadar_mapping_icd_to_group = "smadar_mapping_icd2.txt";
		string smadar_mapping_groups_name = "smadar_mapping_groups.txt";


	

		map<int, int> segal_info;
		if (1 == 1) {
			int count = 0;
			string fname = "segal_info_indication.txt";
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					int code = stoi(fields[1]);

					int ind = 0;
					if (code > 0) {
						segal_info[id] = 1;
					}
				}
			}
			fprintf(stderr, "segal_info %i\n", (int)segal_info.size());
		}



		map<string, string> map_from_y_to_icd;
		map<string, string> map_icd_to_desc;
		//code desc
		if (1 == 1) {
			int count = 0;
			string fname = "C:\\corona_segal\\diag_code_desc_from_esma.txt";;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));

					string DIAGNOSIS_CODE = fields[2];
					string DIAGNOSIS_DESC = fields[3];
					string DIAGNOSIS_CODE_Y = fields[4];

					map_from_y_to_icd[DIAGNOSIS_CODE_Y] = DIAGNOSIS_CODE;
					map_icd_to_desc[DIAGNOSIS_CODE] = DIAGNOSIS_DESC;


				}
			}
		}


		//read tests
		map<int, int> test_neg_first;
		map<int, int> test_pos_first;
		map<int, int>  test_any;
		map<int, int> id_gender;
		map<int, int> id_age;
		if (1 == 1) {
			map<string, int> tests_names_map;
			int count = 0;
			string fname = tests;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int jj = 0; jj < fields.size(); jj++)
							tests_names_map[fields[jj]] = jj;
						continue;
					}

					split(fields, curr_line, boost::is_any_of("\t"));
					//int id = stoi(fields[0]);

					int age = stoi(fields[5]);
					int gender = stoi(fields[6]);


					int id = stoi(fields[tests_names_map["RANDOM_ID"]]);

					id_age[id] = age;
					id_gender[id] = gender;


					string SAMPLE_RESULT_DATE = fields[tests_names_map["SAMPLE_RESULT_DATE"]];
					string execution_date = fields[tests_names_map["SAMPLE_EXECUTION_DATE"]];
					int TEST_RESULT_CD = stoi(fields[tests_names_map["TEST_RESULT_CD"]]);
					string final_date;
					if (execution_date == "1800-01-01 00:00:00.000" && SAMPLE_RESULT_DATE == "1/1/2020") continue;
					if (execution_date == "1800-01-01 00:00:00.000" && convert_date(SAMPLE_RESULT_DATE) == 20200101) continue;

					int new_date;
					if (execution_date != "1800-01-01 00:00:00.000" && convert_date(execution_date) <= datefilter) {
						final_date = execution_date;
						new_date = convert_date(final_date);
					}
					else {
						final_date = SAMPLE_RESULT_DATE;
						new_date = convert_date(final_date);
					}

					if (TEST_RESULT_CD == 1) {
						test_any[id] = 1;
						if (test_neg_first.find(id) == test_neg_first.end())
							test_neg_first[id] = new_date;
						else if (new_date < test_neg_first[id]) test_neg_first[id] = new_date;
					}

					if (TEST_RESULT_CD == 2) {
						test_any[id] = 1;
						if (test_pos_first.find(id) == test_pos_first.end())
							test_pos_first[id] = new_date;
						else if (new_date < test_pos_first[id]) test_pos_first[id] = new_date;
					}
				}
			}
		}


		//read-pop_file 
		map<int, int> map_id_doctor;
		if (1 == 1) {
			int count = 0;
			string fname = "C:\\corona_segal\\doctors1.txt";;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0)) {
					count++;
					if (count == 1) continue;
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[0]);
					map_id_doctor[id] = 1;
				}
			}
		}



		vector<int> vec_neg_dates;
		for (auto it = test_neg_first.begin(); it != test_neg_first.end(); it++) {
			vec_neg_dates.push_back(it->second);
		}
		int n_vec_neg_dates = (int)vec_neg_dates.size();

		//read_diagnosis
		map<int, int> id_diagnosys_corona_date;
		if (1 == 2) {
			map<string, int> map_diagnosis_names;
			map<int, int>id_diagnosys_corona;

			int count = 0;
			string fname = diag_fname;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) {
						split(fields, curr_line, boost::is_any_of("\t"));
						for (int jj = 0; jj < fields.size(); jj++)
							map_diagnosis_names[fields[jj]] = jj;
						continue;
					}
					split(fields, curr_line, boost::is_any_of("\t"));
					int id = stoi(fields[map_diagnosis_names["RANDOM_ID"]]);
					int DIAGNOSIS_TYPE_CODE = stoi(fields[map_diagnosis_names["DIAGNOSIS_TYPE_CODE"]]);
					string VISIT_DATE = fields[map_diagnosis_names["VISIT_DATE"]];
					int VISIT_DATE_ki = convert_date(VISIT_DATE);

					if (DIAGNOSIS_TYPE_CODE == 3 && id_diagnosys_corona.find(id) == id_diagnosys_corona.end()) {
						id_diagnosys_corona[id] = 1;
						id_diagnosys_corona_date[id] = VISIT_DATE_ki;
					}
					else if (DIAGNOSIS_TYPE_CODE == 3 && id_diagnosys_corona.find(id) != id_diagnosys_corona.end()) {
						if (VISIT_DATE_ki < id_diagnosys_corona_date[id])  id_diagnosys_corona_date[id] = VISIT_DATE_ki;
					}
				}
			}
		}



		map<int, int> corona_pos_final;
		for (auto it = test_pos_first.begin(); it != test_pos_first.end(); it++) {
			int id = it->first;
			int pos_date = it->second;
			corona_pos_final[id] = pos_date;
			if (id_diagnosys_corona_date.find(id) != id_diagnosys_corona_date.end()) {
				if (id_diagnosys_corona_date[id] < pos_date) corona_pos_final[id] = id_diagnosys_corona_date[id];
			}
		}
		for (auto it = id_diagnosys_corona_date.begin(); it != id_diagnosys_corona_date.end(); it++) {
			int id = it->first;
			int diag_date = it->second;
			if (corona_pos_final.find(id) == corona_pos_final.end()) corona_pos_final[id] = diag_date;

			if (test_any.find(id) == test_any.end()) test_any[id] = 1;
		}





		//read karni group
		map<string, string> map_karni_group;
		if (1 ==2) {
			string fname = "";// karni_kod_mapping_fname;
			int count = 0;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					count++;
					if (count == 1) continue;
					else {
						split(fields, curr_line, boost::is_any_of("\t"));
						string icd_code = fields[0];
						string maccabi_code = fields[1];
						string group = fields[2];
						trim(group);
						//fprintf(stderr, "%s\n", curr_line.c_str());
						//getchar();

						if (group == "MEMORY DISTURBANCE" || group == "Speech Disturbance" || group == "symp_bilbul") group = "symp_bilbul_Speech_memory";


						if (icd_code != "NULL" && group != "0") map_karni_group[icd_code] = group;
						if (maccabi_code != "NULL" && group != "0") map_karni_group[maccabi_code] = group;
					}
				}
			}
		}


		map<string, string> map_icd_to_group_name;
		map<int, string> map_group_id_to_group_name;
		
		if (1 == 1) {
			string fname = smadar_mapping_groups_name;
			int count = 0;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
					split(fields, curr_line, boost::is_any_of("\t"));
					int group_id = stoi(fields[0]);
					string group_name = fields[1];
					trim(group_name);
					map_group_id_to_group_name[group_id] = group_name;
				}
			}
		}
		
		if (1 == 1) {
			string fname = smadar_mapping_icd_to_group;
			int count = 0;
			fprintf(stderr, "read %s \n", fname.c_str());
			ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
			while (getline(inf, curr_line)) {
				if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
						split(fields, curr_line, boost::is_any_of("\t"));
						int group_id = stoi(fields[0]);
						string icd_code = fields[1];
						trim(icd_code);
						map_icd_to_group_name[icd_code] = map_group_id_to_group_name[group_id];
				}
			}
		}

		for (auto it = map_icd_to_group_name.begin(); it != map_icd_to_group_name.end(); it++) {
			string s1 = it->first;
			string s2 = it->second;

			//fprintf(stderr, "====%s===  =====%s=====  \n", s1.c_str(), s2.c_str());
			//getchar();
		}
		//fprintf(stderr, "map_icd_to_group_name : %i \n", (int)map_icd_to_group_name.size());
		//getchar();


			for (int mm = 2; mm <3; mm++) {
			//for (int mm = 0; mm < 1; mm++) {

				//for (int mm = 2; mm <3; mm++) {


				string fname;
				FILE* fout = NULL;



				if (mm == 0) {
					fname = symp_dir + "data_doctor_table1_id_level.txt";
					fout = fopen(fname.c_str(), "w");
					fprintf(fout, "%s\t%s\t%s\n", "param_name", "corona_negative", "corona_positive");
				}
				else if (mm == 1) {
					fname = symp_dir + "data_doctor_table1_diag_level.txt";
					fout = fopen(fname.c_str(), "w");
					fprintf(fout, "%s\t%s\t%s\n", "param_name", "corona_negative", "corona_positive");
				}
				else if (mm == 2) {
					fname = symp_dir + "data_doctor_table1_id_level_before_test_0_21_or.txt";
					fout = fopen(fname.c_str(), "w");
					fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "corona_negative"
						, "corona_positive", "noind_corona_negative", "noind_corona_positive"
						, "or_positive_negative", "CI"
						, "rr_positive_negative", "CI"
					);
				}
				else if (mm == 3) {
					fname = symp_dir + "data_doctor_table1_id_level_all_time_or.txt";
					fout = fopen(fname.c_str(), "w");
					fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "corona_negative"
						, "corona_positive", "noind_corona_negative", "noind_corona_positive"
						, "or_positive_negative", "CI", "or_positive_no_test", "CI"
					);
				}
				else if (mm == 4) {
					fname = symp_dir + "data_doctor_table1_id_level_before_test_0_99_or.txt";
					fout = fopen(fname.c_str(), "w");
					fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "corona_negative"
						, "corona_positive", "noind_corona_negative", "noind_corona_positive"
						, "or_positive_negative", "CI", "or_positive_no_test", "CI"
					);
				}
				else if (mm == 5) {
					fname = symp_dir + "data_doctor_table1_id_level_after_test_0_999_or.txt";
					fout = fopen(fname.c_str(), "w");
					fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "param_name", "corona_negative"
						, "corona_positive", "noind_corona_negative", "noind_corona_positive"
						, "or_positive_negative", "CI"
						, "rr_positive_negative", "CI"
					);
				}



				for (int tt = 0; tt <3; tt++) {

					fprintf(stderr, "mm: %i tt : %i \n", mm, tt);

					string age_s = "";
					if (tt==0) age_s = "Age_all_";
					else if (tt == 1) age_s = "Age_0_17_";
					else if (tt == 2) age_s = "Age_18_100_";
					else {
						fprintf(stderr, "eror ... \n");
						getchar();
					}


				//*** read symptom maccabi file 
				map<int, map<int, vector<string>>> id_date_diags;
				map<int, int> id_corona_ind;
				map<int, int> map_id_diag;
				map<int, int> map_id_diag_smadar;
				map<int, int> map_id_diag_pos;
				
				map<int, map<int, int>> map_id_vistis;
				map<int, map<int, int>> map_id_vistis_before;
				map<int, map<int, int>> map_id_vistis_after;

				map<int, map<int, int>> map_id_vistis_smadar_before_all_time;
				map<int, map<int, int>> map_id_vistis_smadar_after_all_time;

				map<int, map<int, int>> map_id_vistis_smadar;
				map<int, map<int, int>> map_id_vistis_smadar_before;
				map<int, map<int, int>> map_id_vistis_smadar_after;

				int min_date, max_date;
				map<string, int> map_icd_count;
				map<int, int> map_calc_age;
				if (1 == 1) {
					int count = 0;
					int count_with_group = 0;
					int count_no_group = 0;
					string fname = symp_id_fname;
					fprintf(stderr, "read %s \n", fname.c_str());
					ifstream inf(fname); if (!inf) { fprintf(stderr, "======Can't open file %s\n", fname.c_str()); return -1; }
					while (getline(inf, curr_line)) {
						if ((curr_line.size() > 0) && (curr_line[0] != '#')) {
							count++;
							if (count == 1) continue;
							//split(fields, curr_line, boost::is_any_of("\t"));
							split(fields, curr_line, boost::is_any_of(","));
							int id = stoi(fields[0]);

							//if (map_id_doctor.find(id) == map_id_doctor.end()) continue;
							if (tt == 0) {

							}
							else if (tt == 1) {
								if (id_age[id] >=18) continue;
							}
							else if (tt == 2) {
								if (id_age[id] <= 17) continue;
							}

							
							
							
							//if (id_age[id] >= 18) {
								//continue;
							//}
							//else {
								//continue;
							//}

							string symp_code = fields[1];
							trim(symp_code);

							


							string symp_code_icd = "";
							if (symp_code.substr(0, 1) == "Y") {
								if (map_from_y_to_icd.find(symp_code) == map_from_y_to_icd.end()) {
									fprintf(stderr, "cant find maccabi code %s \n", symp_code.c_str());
									getchar();
								}
								symp_code_icd = map_from_y_to_icd[symp_code];
							}
							else {
								symp_code_icd = symp_code;
							}


							string symp_code_icd_with_desc = "";
							if (map_icd_to_desc.find(symp_code_icd) == map_icd_to_desc.end()) {
								//fprintf(stderr, "cant find icd desc %s orig %s \n", symp_code_icd.c_str(), fields[1].c_str());
								//fprintf(stderr, " %s \n", curr_line.c_str());
								//getchar();
								symp_code_icd_with_desc = symp_code_icd;
							}
							else {
								symp_code_icd_with_desc = map_icd_to_desc[symp_code_icd];
							}


							//fprintf(stderr, "symp_code %s \n", symp_code.c_str());
							//getchar();

							int date = stoi(fields[2]);

							if (count == 2) {
								min_date = date;
								max_date = date;
							}
							else {
								if (date < min_date) min_date = date;
								if (date > max_date) max_date = date;
							}

							int corona_ind = -9;
							int corona_date = -9;
							int corona_diff = -9;
							if (corona_pos_final.find(id) != corona_pos_final.end()) {
								corona_ind = 1;
								corona_date = corona_pos_final[id];
								corona_diff = datediff(corona_date, date);
							}
							else if (test_neg_first.find(id) != test_neg_first.end()) {
								corona_ind = 0;
								corona_date = test_neg_first[id];
								corona_diff = datediff(corona_date, date);
							}
							else {
								continue;
							}

							map_icd_count[symp_code]++;
							map_calc_age[id] = id_age[id];

							//if (symp_code_icd == "250.90") {
								//fprintf(stderr, "%i %i \n", id, corona_ind);
								//getchar();
							//}


							//run pre...
							if (mm == 2) {
								if (corona_diff > 0 || corona_diff < -21) continue;
								//if (corona_diff > 0) continue;
								//if (corona_diff > 0) continue;
							}
							else if (mm == 4) {
								if (corona_diff > 0) continue;
							}
							else if (mm == 5) {
								//if (corona_diff <= 0 || corona_diff > 21) continue;
								if (corona_diff <= 0) continue;
							}

							if (corona_ind == 1) map_id_diag_pos[id] = 1;

							id_corona_ind[id] = corona_ind;
							map_id_diag[id]++;
							map_id_vistis[id][date] = 1;


							if (corona_diff >= -21 && corona_diff <= 0) map_id_vistis_before[id][date] = 1;
							if (corona_diff > 0 && corona_diff <= 21) map_id_vistis_after[id][date] = 1;


							//string symp_group = "Other";
							//if (map_karni_group.find(symp_code) != map_karni_group.end()) {
								//symp_group = map_karni_group[symp_code];
								//count_with_group++;
							//}
							//else {
								//count_no_group++;
							//}

							//barak
							//string symp_group = symp_code_icd_with_desc;

							//if (id == 2546481) {
								//fprintf(stderr, "symp_code_icd  %s \n", symp_code_icd.c_str());
								//fprintf(stderr, "symp_group  %s \n", map_icd_to_group_name[symp_code_icd].c_str());
								//getchar();
							//}


							if (map_icd_to_group_name.find(symp_code_icd) != map_icd_to_group_name.end()) {
								string symp_group = map_icd_to_group_name[symp_code_icd];
								id_date_diags[id][date].push_back(symp_group);
								map_id_vistis_smadar[id][date] = 1;

								if (corona_diff>=-21 && corona_diff <=0) map_id_vistis_smadar_before[id][date]=1;
								if (corona_diff >0 && corona_diff <= 21) map_id_vistis_smadar_after[id][date] = 1;

								if (corona_diff <= 0) map_id_vistis_smadar_before_all_time[id][date] = 1;
								if (corona_diff > 0) map_id_vistis_smadar_after_all_time[id][date] = 1;


								map_id_diag_smadar[id]++;
							}
						}
					}
					fprintf(stderr, "count_with_group %i\n", count_with_group);
					fprintf(stderr, "count_no_group %i\n", count_no_group);
				}
				fprintf(stderr, "map_id_diag : %i \n", (int)map_id_diag.size());
				fprintf(stderr, "map_id_diag_pos : %i \n", (int)map_id_diag_pos.size());

				fprintf(stderr, "smadar min_date %i  \n", min_date);
				fprintf(stderr, "smadar max_date %i  \n", max_date);


				vector<float> calc_age;
				//FILE* fout_smadar = fopen("smadar_ids.txt", "w");
				for (auto it = map_calc_age.begin(); it != map_calc_age.end(); it++) {
					int id = it->first;
					int age = it->second;
					int age_ind = 0;
					if (age >= 18) age_ind = 1;
					int corona_ind = id_corona_ind[id];
					calc_age.push_back((float)age);
					//fprintf(fout_smadar, "%i\t%i\t%i\n", id, corona_ind, age_ind);
				}
				//fclose(fout_smadar);
				fprintf(stderr, "calc_age %i \n", (int) calc_age.size());
				fprintf(stderr, "calc_age mean %f \n", get_mean(calc_age));
				fprintf(stderr, "calc_age sdv %f \n", get_sdv(calc_age));
				//getchar();

				//FILE* fout_qa_icd = fopen("qa_icd.txt", "w");
				//fprintf(fout_qa_icd, "%s\t%s\n", "icd", "count");
				//for (auto it = map_icd_count.begin(); it != map_icd_count.end(); it++) {
					//fprintf(fout_qa_icd, "%s\t%i\n", it->first.c_str(), it->second);
				//}
				//fclose(fout_qa_icd);

				//getchar();
				//return 1;


				map<string, int> map_symp_names1;
				for (auto it = id_date_diags.begin(); it != id_date_diags.end(); it++) {
					int numerator = it->first;
					for (auto it1 = id_date_diags[numerator].begin(); it1 != id_date_diags[numerator].end(); it1++) {
						int milui_date = it1->first;
						for (int jj = 0; jj < id_date_diags[numerator][milui_date].size(); jj++) {
							string signal = id_date_diags[numerator][milui_date][jj];


							if (corona_pos_final.find(numerator) != corona_pos_final.end())
								map_symp_names1[signal] = 1;
						}
					}
				}

				int temp_name = 0;
				for (auto it = map_symp_names1.begin(); it != map_symp_names1.end(); it++) {
					string sig = it->first;
					map_symp_names1[sig] = temp_name;
					temp_name++;
				}

				//fprintf(stderr, "map_symp_names1 %i \n", (int)map_symp_names1.size());
				//getchar();

				//**** id level ***/
				if (mm == 0 || mm == 2 || mm == 3 || mm == 4 || mm == 5) {
					map<int, int> map_count_id;
					map<int, map<string, map<int, int>>> corona_count_param;
					map<int, map<string, map<int, vector<float>>>> corona_avg_param;
					//string fname = symp_dir + "table_1_diag_id_level.txt";
					//FILE *fout = fopen(fname.c_str(), "w");
					//fprintf(fout, "%s\t%s\t%s\n", "param", "corona_neagative", "corona_positive");



					for (auto it = map_id_diag.begin(); it != map_id_diag.end(); it++) {
						int count_param = 0;
						int id = it->first;
						int corona_ind = id_corona_ind[id];
						int gender = id_gender[id];
						int age = id_age[id];
						int num_of_diag = map_id_diag[id];
						int info = 0;
						if (segal_info.find(id) != segal_info.end())info++;
						
						
						int num_of_visits = (int)map_id_vistis[id].size();
						int num_of_visits_before = (int)map_id_vistis_before[id].size();
						int num_of_visits_after = (int)map_id_vistis_after[id].size();

						int num_of_visits_smadar_before_all_time = (int)map_id_vistis_smadar_before_all_time[id].size();
						int  num_of_visits_smadar_after_all_time = (int)map_id_vistis_smadar_after_all_time[id].size();



						int num_of_visits_smadar = (int)map_id_vistis_smadar[id].size();
						int num_of_visits_smadar_before = (int)map_id_vistis_smadar_before[id].size();
						int num_of_visits_smadar_after = (int)map_id_vistis_smadar_after[id].size();


						int smadar_ind = 0;
						if (map_id_diag_smadar.find(id) != map_id_diag_smadar.end())smadar_ind = 1;

						corona_count_param[count_param++]["count_id"][corona_ind]++;
						corona_count_param[count_param++]["count_id_with_info"][corona_ind]+=info;
						corona_count_param[count_param++]["count_id_smadar"][corona_ind]+=smadar_ind;
						map_count_id[corona_ind]++;
						if (mm == 0) {
							corona_count_param[count_param++]["count_diags"][corona_ind] += num_of_diag;
							corona_count_param[count_param++]["count_visits"][corona_ind] += num_of_visits;

							corona_count_param[count_param++]["count_visits_before"][corona_ind] += num_of_visits_before;
							corona_count_param[count_param++]["count_visits_after"][corona_ind] += num_of_visits_after;


							corona_count_param[count_param++]["count_visits_smadar"][corona_ind] += num_of_visits_smadar;

							corona_count_param[count_param++]["count_visits_smadar_before"][corona_ind] += num_of_visits_smadar_before;
							corona_count_param[count_param++]["count_visits_smadar_after"][corona_ind] += num_of_visits_smadar_after;

							corona_count_param[count_param++]["count_visits_smadar_before_all_time"][corona_ind] += num_of_visits_smadar_before_all_time;
							corona_count_param[count_param++]["count_visits_smadar_after_all_time"][corona_ind] += num_of_visits_smadar_after_all_time;



							if (gender == 1) corona_count_param[count_param]["count_female"][corona_ind] ++; count_param++;
							if (gender == 0) corona_count_param[count_param]["count_male"][corona_ind] ++; count_param++;
							corona_avg_param[count_param]["avg_sdv_num_of_diags"][corona_ind].push_back((float)num_of_diag); count_param++;
							corona_avg_param[count_param]["avg_sdv_num_of_visits"][corona_ind].push_back((float)num_of_visits); count_param++;
							corona_avg_param[count_param]["avg_sdv_num_of_age"][corona_ind].push_back((float)age); count_param++;
						}



						map<string, int> map_symp_names1x;
						for (auto it1 = id_date_diags[id].begin(); it1 != id_date_diags[id].end(); it1++) {
							int milui_date = it1->first;
							for (int jj = 0; jj < id_date_diags[id][milui_date].size(); jj++) {
								string signal = id_date_diags[id][milui_date][jj];
								if (id == 1922961 && map_symp_names1.find(signal) != map_symp_names1.end()) {
									fprintf(stderr, "%i %i %s \n", id, milui_date, signal.c_str());
								}
								map_symp_names1x[signal] = 1;
							}
						}



						for (auto it2 = map_symp_names1.begin(); it2 != map_symp_names1.end(); it2++) {
							string symp_to_check = it2->first;
							int ind = 0;
							if (map_symp_names1x.find(symp_to_check) != map_symp_names1x.end()) ind = 1;
							//for (auto it1 = id_date_diags[id].begin(); it1 != id_date_diags[id].end(); it1++) {
								//int milui_date = it1->first;
								//for (int jj = 0; jj < id_date_diags[id][milui_date].size(); jj++) {
									//string signal = id_date_diags[id][milui_date][jj];
									//if (signal == symp_to_check) ind = 1;
								//}
							//}
							corona_count_param[count_param][symp_to_check][corona_ind] += ind;
							count_param++;
						}

					}

					if (mm == 0) {
						for (auto it0 = corona_count_param.begin(); it0 != corona_count_param.end(); it0++) {
							int code = it0->first;
							for (auto it = corona_count_param[code].begin(); it != corona_count_param[code].end(); it++) {
								string param = it->first;
								fprintf(fout, "%s\t%s\t", age_s.c_str(), param.c_str());
								//int count_ind_no_test = 0, count_ind_negative = 0, count_ind_positive = 0, count_noind_no_test = 0, count_noind_negative = 0, count_noind_positive = 0;
								for (auto it1 = corona_count_param[code][param].begin(); it1 != corona_count_param[code][param].end(); it1++) {
									int corona_ind = it1->first;
									int count_id = it1->second;
									fprintf(fout, "%i\t", count_id);
								}
								fprintf(fout, "\n");
							}
						}
					}

					if (mm == 0) {
						for (auto it0 = corona_avg_param.begin(); it0 != corona_avg_param.end(); it0++) {
							int code = it0->first;

							for (auto it = corona_avg_param[code].begin(); it != corona_avg_param[code].end(); it++) {
								string param = it->first;
								fprintf(fout, "%s\t%s\t", age_s.c_str(), param.c_str());
								for (auto it1 = corona_avg_param[code][param].begin(); it1 != corona_avg_param[code][param].end(); it1++) {
									int corona_ind = it1->first;
									float avg = get_mean(it1->second);
									float sdv = get_sdv(it1->second);
									fprintf(fout, "%.2f(%.2f)\t", avg, sdv);
								}
								fprintf(fout, "\n");
							}
						}
					}


					if (mm == 2 || mm == 4 || mm == 3 || mm == 5) {
						for (auto it0 = corona_count_param.begin(); it0 != corona_count_param.end(); it0++) {
							int code = it0->first;
							for (auto it = corona_count_param[code].begin(); it != corona_count_param[code].end(); it++) {
								string param = it->first;
								fprintf(fout, "%s\t%s\t", age_s.c_str(), param.c_str());
								int count_ind_no_test = 0, count_ind_negative = 0, count_ind_positive = 0, count_noind_no_test = 0, count_noind_negative = 0, count_noind_positive = 0;
								for (auto it1 = corona_count_param[code][param].begin(); it1 != corona_count_param[code][param].end(); it1++) {
									int corona_ind = it1->first;
									int count_id = it1->second;
									if (corona_ind == 0) {
										count_ind_negative = count_id;
										count_noind_negative = map_count_id[corona_ind] - count_ind_negative;
									}
									else if (corona_ind == 1) {
										count_ind_positive = count_id;
										count_noind_positive = map_count_id[corona_ind] - count_ind_positive;
									}

									fprintf(fout, "%i\t", count_id);

								}


								if (param != "count_id") {
									fprintf(fout, "%i\t%i\t", count_noind_negative, count_noind_positive);
									vector<float>  or_positive_negative = get_or(count_ind_positive, count_noind_positive, count_ind_negative, count_noind_negative);
									fprintf(fout, "%.2f\t[%.2f-%.2f]\t", or_positive_negative[0], or_positive_negative[1], or_positive_negative[2]);

									vector<float>  rr_positive_negative = get_rr(count_ind_positive, count_noind_positive, count_ind_negative, count_noind_negative);
									fprintf(fout, "%.2f\t[%.2f-%.2f]\t", rr_positive_negative[0], rr_positive_negative[1], rr_positive_negative[2]);

								}
								fprintf(fout, "\n");
							}

						}
					}
				}

				//**** diag level ****//
				if (mm == 1) {

					map<int, map<string, map<int, int>>> corona_count_param;
					//string fname = symp_dir + "table_1_diag_level.txt";
					//FILE *fout = fopen(fname.c_str(), "w");
					//fprintf(fout, "%s\t%s\t%s\n", "param", "corona_neagative", "corona_positive");

					//int count_param = 0;
					//for (auto it2 = map_symp_names1.begin(); it2 != map_symp_names1.end(); it2++) {
						//string symp_to_check = it2->first;
					for (auto it = id_date_diags.begin(); it != id_date_diags.end(); it++) {
						int id = it->first;
						int corona_ind = id_corona_ind[id];
						for (auto it1 = id_date_diags[id].begin(); it1 != id_date_diags[id].end(); it1++) {
							int milui_date = it1->first;
							for (int jj = 0; jj < id_date_diags[id][milui_date].size(); jj++) {
								string signal = id_date_diags[id][milui_date][jj];
								int count_param = map_symp_names1[signal];
								//if (signal == symp_to_check) corona_count_param[count_param][symp_to_check][corona_ind]++;
								if (map_symp_names1.find(signal) != map_symp_names1.end()) corona_count_param[count_param][signal][corona_ind]++;
							}
						}
					}
					//count_param++;
				//}
					fprintf(stderr, "quest level ...\n");
					for (auto it0 = corona_count_param.begin(); it0 != corona_count_param.end(); it0++) {
						int code = it0->first;
						for (auto it = corona_count_param[code].begin(); it != corona_count_param[code].end(); it++) {
							string param = it->first;
							fprintf(fout, "%s\t%s\t", age_s.c_str(), param.c_str());
							for (auto it1 = corona_count_param[code][param].begin(); it1 != corona_count_param[code][param].end(); it1++) {
								int corona_ind = it1->first;
								int count_id = it1->second;
								fprintf(fout, "%i\t", count_id);
							}
							fprintf(fout, "\n");
						}
					}

				}


			}
			fclose(fout);
		}


		//moving average 
		if (1 == 2) {
			map<int, map<int, vector<string>>> id_date_diags;
			fprintf(stderr, "segal anlysis 1 ...\n");
			//corona pos / diff / signal / count
			string dir_graph = "D:\\projects\\corona_maccabi\\symptoms\\temp_garphs\\";
			string fname = dir_graph + "segal_new_from_doctor.txt";
			FILE* fout = fopen(fname.c_str(), "w");
			for (int mm = 1; mm <= 9; mm += 2) {
				//int curr_win = 7;
				int curr_win = mm;

				map<string, map<int, map<int, int>>> map_pos_diff_signal_count;
				map<int, map<int, int>> map_pos_diff_count;

				for (int hh = -14; hh <= 14; hh++) {
					int curr_diff = hh;
					int curr_diff_from = curr_diff - ((curr_win - 1) / 2);
					int curr_diff_to = curr_diff + ((curr_win - 1) / 2);
					for (auto it = id_date_diags.begin(); it != id_date_diags.end(); it++) {
						int numerator = it->first;

						int age = id_age[numerator];
						//if (age < 18) continue;

						int corona_test_ind = -9;
						int corona_test_date = -9;
						{
							if (corona_pos_final.find(numerator) != corona_pos_final.end()) {
								corona_test_ind = 1;
								corona_test_date = corona_pos_final[numerator];
							}
							else if (test_neg_first.find(numerator) != test_neg_first.end()) {
								corona_test_ind = 0;
								corona_test_date = test_neg_first[numerator];
							}
						}


						int milui_in = 0;
						map<string, int> signal_in;
						for (auto it1 = id_date_diags[numerator].begin(); it1 != id_date_diags[numerator].end(); it1++) {
							int milui_date = it1->first;
							int milui_diff = datediff(corona_test_date, milui_date);
							if (milui_diff >= curr_diff_from && milui_diff <= curr_diff_to) {
								milui_in = 1;
								for (int jj = 0; jj < id_date_diags[numerator][milui_date].size(); jj++) {
									string signal = id_date_diags[numerator][milui_date][jj];
									signal_in[signal] = 1;
								}
							}
						}
						map_pos_diff_count[corona_test_ind][curr_diff] += milui_in;
						for (auto it1 = signal_in.begin(); it1 != signal_in.end(); it1++) {
							string signal = it1->first;
							map_pos_diff_signal_count[signal][corona_test_ind][curr_diff]++;
						}
					}
				}

				if (1 == 1) {

					for (auto it = map_pos_diff_signal_count.begin(); it != map_pos_diff_signal_count.end(); it++) {
						string symp = it->first;
						//if (symp_in.find(symp) == symp_in.end()) continue;
						//symp = "symp_keev_rosh";
						for (auto it1 = map_pos_diff_signal_count[symp].begin(); it1 != map_pos_diff_signal_count[symp].end(); it1++) {
							int corona_ind = it1->first;
							for (auto it2 = map_pos_diff_signal_count[symp][corona_ind].begin(); it2 != map_pos_diff_signal_count[symp][corona_ind].end(); it2++) {
								int diff_to_test = it2->first;
								int count = it2->second;
								fprintf(fout, "%i\t%i\t%i\t%s\t%i\n", curr_win, corona_ind, diff_to_test, symp.c_str(), count);
								fprintf(stderr, "%i\t%i\t%i\t%s\t%i\n", curr_win, corona_ind, diff_to_test, symp.c_str(), count);
							}
						}
						//break;
					}
				}


				for (auto it = map_pos_diff_signal_count.begin(); it != map_pos_diff_signal_count.end(); it++) {
					string symp = it->first;
					//if (symp_in.find(symp) == symp_in.end()) continue;
					//symp = "symp_keev_rosh";
					string plot_name = symp + " win " + to_string(curr_win) + " days";
					string plot_name1 = "Doctor Visit";
					plot_bar_segal_symp(plot_name, map_pos_diff_signal_count[symp], map_pos_diff_count, plot_name1, symp_dir);
					//break;
				}

				//break;
			}
			fclose(fout);
			//corona pos / diff / signal / count
			//map<int, map<int, map<string, int>>> map_pos_diff_signal_count;

		}
	}
