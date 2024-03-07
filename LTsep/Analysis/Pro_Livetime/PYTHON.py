import sys
script_name = sys.argv[0]
run = sys.argv[1]

file_path = '/lustre19/expphy/volatile/hallc/c-kaonlt/vijay/REPORT_OUTPUT/Production/Kaon_replay_production_'+run+'_-1.report'  

with open(file_path, 'r') as file:
    line = file.readline()  # Read a single line from the file
    for line in file:

        coin_rate_line = "KLT_COIN_Trigger_Rate"
        if coin_rate_line in line:            
            coin_rates = line.split()
            coin_rate_index = 2
            
            if coin_rate_index < len(coin_rates):
                coin_rate_value = coin_rates[coin_rate_index]

                print(coin_rate_value)
            else:
                print("Invalid word index.")

        EDTM_live_line = "KLT_Non_Scaler_EDTM_Live_Time"
        if EDTM_live_line in line: 
            EDTM_lives = line.split()
            EDTM_live_index = 2
            EDTM_live_err_index = 4
            
            if EDTM_live_index < len(EDTM_lives):
                EDTM_live_value = EDTM_lives[EDTM_live_index]
                EDTM_live_value_err = EDTM_lives[EDTM_live_err_index]
                
                print(EDTM_live_value)
                print(EDTM_live_value_err)
            else:
                print("Invalid word index.")
                    
        CPU_live_line = "KLT_COIN_CPULT"
        if CPU_live_line in line:            
            CPU_lives = line.split()
            CPU_live_index = 2
            CPU_live_err_index = 4
            
            if CPU_live_index < len(CPU_lives):
                CPU_live_value = CPU_lives[CPU_live_index]
                CPU_live_value_err = CPU_lives[CPU_live_err_index]

                print(CPU_live_value)
                print(CPU_live_value_err)
            else:
                print("Invalid word index.")
