import numpy, sys

from ROOT import TCanvas, TGraph, TGraphErrors, TF1, TLegend, TText, TLatex, TLine, TFile, TH1F
from math import sqrt, exp


peak = TH1F()



def Setting_by_setting(file_name):
		
    missmass = []

    #	scale_factor = 20
    #	scale_factor = 120
    #	scale_factor = 1
    scale_factor = 1

    #/*--------------------------------------------------*/
    # Loading the 

 
    t_bin_file = open('t_bin_interval', 'r')

    f_str = t_bin_file.readline()
    print(f_str.split())
    t_bin_file.close()

    t_bin_num   = int(f_str.split()[1])
    phi_bin_num = int(f_str.split()[2])



#    print(t_bin_num, phi_bin_num)
#    sys.exit(0)

    dum_name = "yields." + file_name  + ".dummy.root"
    tgt_name = "yields." + file_name  + ".target.root"
    sim_name = "yields." + file_name  + ".sim.root"

    dum_file = TFile(dum_name,      "READ")
    tgt_file = TFile(tgt_name,      "READ")
    sim_file = TFile(sim_name, "READ")




#    plot_name_list = ["missmass", "hsdelta", "Q2", "W", "hsxptar", "hsyptar", "hsytar", "t", "ssytar", "ssyptar", "ssxptar", "th_pq", "phi_pq", "Pmpar", "Pmper", "Pmoop", "Em", "Pm"]
    

    print(file_name)

#    plot_name_list = ["missmass", "hsdelta", "Q2", "W", "hsytar", "ssytar"]

#    plot_name_list = ["missmass", "hsdelta", "Q2", "W", "hsytar", "hsxptar", "hsyptar", "ssytar", "ssxptar", "ssyptar", "phi_pq", "Pmpar", "Pmper", "Pmoop", "u", "t"]


#    plot_name_list = ["missmass", "hsdelta", "Q2", "W", "hsxtar", "hsytar", "hsxptar", "hsyptar", "ssxtar", "ssytar", "ssxptar", "ssyptar", "phi_pq", "Pmpar", "Pmper", "Pmoop", "u"]

#    plot_name_list = ["missmass_dia", "hsdelta_dia", "Q2_dia", "W_dia", "hsytar_dia", "hsxptar_dia", "hsyptar_dia", "ssytar_dia", "ssxptar_dia", "ssyptar_dia", "phi_pq_dia", "Pmpar_dia", "Pmper_dia", "Pmoop_dia", "u_dia", "t_dia"]

    plot_name_list = ["missmass", "hsdelta", "Q2", "W", "hsytar", "hsxptar", "hsyptar", "ssytar", "ssxptar", "ssyptar", "phi_pq", "Pmpar", "Pmper", "Pmoop", "u", "t", "Em", "Pm"]

    file_out = TFile("final_" + file_name + "_out_put.root", "RECREATE")


    for x in range(len(plot_name_list)):
#    for x in range(1):
    
       missmass[:] = []
    
       print(x, plot_name_list[x])
       plot_name = plot_name_list[x]

       in_plot_name = plot_name + "_dia"



#        if (plot_name == "u"): 
#           missmass_tmp = tgt_file.Get("t")
#           missmass.append(missmass_tmp)
#        
#            missmass_tmp = dum_file.Get("t")
#            missmass.append(missmass_tmp)
# 
#        else:
       missmass_tmp = tgt_file.Get(in_plot_name)
       missmass.append(missmass_tmp)
        
        missmass_tmp = dum_file.Get(in_plot_name)
        missmass.append(missmass_tmp)
    





        missmass_tmp = sim_file.Get(in_plot_name)
        missmass.append(missmass_tmp)
        
        missmass_tmp = missmass[0].Clone()
        missmass_tmp_dummy = missmass[1].Clone()

       missmass_tmp.Add(missmass_tmp_dummy, -1)
        missmass.append(missmass_tmp)





       c1 = TCanvas() 
    
#        if(missmass[0].GetMaximum() > missmass[2].GetMaximum()):
#           missmass[2].SetMaximum(missmass[0].GetMaximum() + 0.2*missmass[0].GetMaximum());
    

        missmass[0].Draw()
    
       missmass[1].SetLineColor(3)
        missmass[1].Draw("same")
    

       missmass[3].SetLineColor(4)
        missmass[3].Draw("same")
    

       print("111111111111")

       missmass[2].Scale(scale_factor)

       missmass[2].SetLineColor(2)
        missmass[2].Draw("same")

    
       file_out.cd()
       c1.Write(plot_name)


       c2 = TCanvas() 

       missmass[3].Draw()



       rightmax = 1.1 * missmass[2].GetMaximum()

       if (rightmax != 0) : 
          scale = missmass[3].GetMaximum()/rightmax
       else:
          scale = 1

       missmass[2].Scale(scale)
        missmass[2].Draw("same")


       rightmax = 1.1 * missmass[4].GetMaximum()

       if (rightmax != 0) : 
          scale = missmass[3].GetMaximum()/rightmax
       else:
          scale = 1

       missmass[4].Scale(scale)
        missmass[4].Draw("same")


       rightmax = 1.1 * missmass[5].GetMaximum()

       if (rightmax != 0) : 
          scale = missmass[3].GetMaximum()/rightmax
       else:
          scale = 1

       missmass[5].Scale(scale)
        missmass[5].Draw("same")




       rightmax = 1.1 * missmass[6].GetMaximum()

       if (rightmax != 0) : 
          scale = missmass[3].GetMaximum()/rightmax
       else:
          scale = 1

       missmass[6].Scale(scale)
        missmass[6].Draw("same")




       rightmax = 1.1 * missmass[7].GetMaximum()

       if (rightmax != 0) : 
          scale = missmass[3].GetMaximum()/rightmax
       else:
          scale = 1

       missmass[7].Scale(scale)
        missmass[7].Draw("same")



#        TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
#        gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
#        axis->SetLineColor(kRed);
#        axis->SetTextColor(kRed);
#        axis->Draw();
       


#        missmass[4].Draw("same")
#        missmass[5].Draw("same")

       c2.Write(plot_name + "_dummy_sub_sim")

       
#        c3 = TCanvas() 
# 
#        missmass[3].Add(missmass[2], -1);
# 
#        missmass[3].Draw("hist")
# 
#        c3.Write(plot_name + "_bg")


#        if plot_name == "missmass":
# #          print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ")
# 
#           Fit_missmass(missmass[2], missmass[3])       
#           
 
#       c4 = TCanvas()
       
       

    # /*--------------------------------------------------*/
    # Missing mass in u-phi bin


    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_missmass")
    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_missmass")
    t_phi_sim = sim_file.Get("t_phi_bin/t_phi_missmass")

#    t_phi_tgt.Write("target_phi")

    c4 = TCanvas()
    c4.Divide(phi_bin_num, t_bin_num)

    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")

    for i in range(1, t_bin_num * phi_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_miss")
        h_tgt_sub = h_tgt.Clone("h_miss_sub")

       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_miss")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_miss")
    
#        h_sim.SetName("h_miss_sim")
#        h_sim_rho.SetName("h_miss_sim_rho")
#        h_sim_xphsp.SetName("h_miss_sim_xphsp")
# 
       h_tgt.SetName("t_phi_target")
       h_tgt_sub.SetName("t_phi_target")
       h_sim.SetName("t_phi_sim")

       h_tgt_sub.Add(h_dum, -1)

       for ii in range(1, h_tgt_sub.GetNbinsX()):

#           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
#              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

#           if(h_dum.GetBinContent(ii) == 0): 
#              sub_error = h_tgt.GetBinError(ii)
#           elif(h_tgt.GetBinContent(ii) == 0): 
#              sub_error = 1.0
#           else: 
#             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)

          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

          h_tgt_sub.SetBinError(ii, sub_error)
 


       c4.cd(i);

       h_sim.SetLineColor(2);
       h_dum.SetLineColor(3);

#       h_tgt.Draw();
#       h_dum.Draw("same")




#       h_tgt.Draw()


       h_tgt_sub.DrawCopy();

       h_temp = c4.cd(i).GetPrimitive("t_phi_target_copy")
       h_temp.SetName("t_phi_target")


       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

       print(i, " asdasdasdasd ")

    c4.Write("missmass_t_phi_bin")





    # /*--------------------------------------------------*/
    # Missing Energy in u-phi bin


    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_Em")
    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_Em")
    t_phi_sim = sim_file.Get("t_phi_bin/t_phi_Em")

#    t_phi_tgt.Write("target_phi")

    c4 = TCanvas()
    c4.Divide(phi_bin_num, t_bin_num)

    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")

    for i in range(1, t_bin_num * phi_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_Em")
        h_tgt_sub = h_tgt.Clone("h_Em_sub")

       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_Em")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_Em")
    
#        h_sim.SetName("h_miss_sim")
#        h_sim_rho.SetName("h_miss_sim_rho")
#        h_sim_xphsp.SetName("h_miss_sim_xphsp")
# 
       h_tgt.SetName("t_phi_target")
       h_tgt_sub.SetName("t_phi_target")
       h_sim.SetName("t_phi_sim")

       h_tgt_sub.Add(h_dum, -1)

       for ii in range(1, h_tgt_sub.GetNbinsX()):

#           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
#              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

#           if(h_dum.GetBinContent(ii) == 0): 
#              sub_error = h_tgt.GetBinError(ii)
#           elif(h_tgt.GetBinContent(ii) == 0): 
#              sub_error = 1.0
#           else: 
#             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)

          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

          h_tgt_sub.SetBinError(ii, sub_error)
 


       c4.cd(i);

       h_sim.SetLineColor(2);
       h_dum.SetLineColor(3);

#       h_tgt.Draw();
#       h_dum.Draw("same")




#       h_tgt.Draw()


       h_tgt_sub.DrawCopy();
       h_temp = c4.cd(i).GetPrimitive("t_phi_target_copy")
       h_temp.SetName("t_phi_target")


       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

       print(i, " asdasdasdasd ")

    c4.Write("Em_t_phi_bin")


    # /*--------------------------------------------------*/
    # Missing Momentum in u-phi bin


    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_Pm")
    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_Pm")
    t_phi_sim = sim_file.Get("t_phi_bin/t_phi_Pm")

#    t_phi_tgt.Write("target_phi")

    c4 = TCanvas()
    c4.Divide(phi_bin_num, t_bin_num)

    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")

    for i in range(1, t_bin_num * phi_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_Pm")
        h_tgt_sub = h_tgt.Clone("h_Pm_sub")

       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_Pm")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_Pm")

    
#        h_sim.SetName("h_miss_sim")
#        h_sim_rho.SetName("h_miss_sim_rho")
#        h_sim_xphsp.SetName("h_miss_sim_xphsp")
# 
       h_tgt.SetName("t_phi_target")
       h_tgt_sub.SetName("t_phi_target")
       h_sim.SetName("t_phi_sim")

       h_tgt_sub.Add(h_dum, -1)

       for ii in range(1, h_tgt_sub.GetNbinsX()):

#           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
#              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

#           if(h_dum.GetBinContent(ii) == 0): 
#              sub_error = h_tgt.GetBinError(ii)
#           elif(h_tgt.GetBinContent(ii) == 0): 
#              sub_error = 1.0
#           else: 
#             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)

          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

          h_tgt_sub.SetBinError(ii, sub_error)
 


       c4.cd(i);

       h_sim.SetLineColor(2);
       h_dum.SetLineColor(3);

#       h_tgt.Draw();
#       h_dum.Draw("same")




#       h_tgt.Draw()


       h_tgt_sub.DrawCopy();
       h_temp = c4.cd(i).GetPrimitive("t_phi_target_copy")
       h_temp.SetName("t_phi_target")


       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

       print(i, " asdasdasdasd ")

    c4.Write("Pm_t_phi_bin")




    # /*--------------------------------------------------*/
    # hsdelta in u-phi bin

    plot_name = "hsdelta"

    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_" + plot_name)
    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_" + plot_name)
    t_phi_sim = sim_file.Get("t_phi_bin/t_phi_" + plot_name)

#    t_phi_tgt.Write("target_phi")

    c4 = TCanvas()
    c4.Divide(phi_bin_num, t_bin_num)

    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")

    for i in range(1, t_bin_num * phi_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_" + plot_name)
        h_tgt_sub = h_tgt.Clone("h_" + plot_name + "_sub")

       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_" + plot_name)
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_" + plot_name)
         
#        h_sim.SetName("h_miss_sim")
#        h_sim_rho.SetName("h_miss_sim_rho")
#        h_sim_xphsp.SetName("h_miss_sim_xphsp")
# 
       h_tgt.SetName("t_phi_target")
       h_tgt_sub.SetName("t_phi_target")
       h_sim.SetName("t_phi_sim")

       h_tgt_sub.Add(h_dum, -1)

       for ii in range(1, h_tgt_sub.GetNbinsX()):

#           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
#              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

#           if(h_dum.GetBinContent(ii) == 0): 
#              sub_error = h_tgt.GetBinError(ii)
#           elif(h_tgt.GetBinContent(ii) == 0): 
#              sub_error = 1.0
#           else: 
#             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)

          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

          h_tgt_sub.SetBinError(ii, sub_error)
 


       c4.cd(i);

       h_sim.SetLineColor(2);
       h_dum.SetLineColor(3);

#       h_tgt.Draw();
#       h_dum.Draw("same")




#       h_tgt.Draw()


       h_tgt_sub.DrawCopy();
       h_temp = c4.cd(i).GetPrimitive("t_phi_target_copy")
       h_temp.SetName("t_phi_target")


       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

       print(i, " asdasdasdasd ")

    c4.Write(plot_name + "_t_phi_bin")




    # /*--------------------------------------------------*/
    # hsxptar in u-phi bin

    plot_name = "hsxptar"

    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_" + plot_name)
    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_" + plot_name)
    t_phi_sim = sim_omega_file.Get("t_phi_bin/t_phi_" + plot_name)

#    t_phi_tgt.Write("target_phi")

    c4 = TCanvas()
    c4.Divide(phi_bin_num, t_bin_num)

    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")

    for i in range(1, t_bin_num * phi_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_" + plot_name)
        h_tgt_sub = h_tgt.Clone("h_" + plot_name + "_sub")

       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_" + plot_name)
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_" + plot_name)

    
#        h_sim.SetName("h_miss_sim")
#        h_sim_rho.SetName("h_miss_sim_rho")
#        h_sim_xphsp.SetName("h_miss_sim_xphsp")
# 
       h_tgt.SetName("t_phi_target")
       h_tgt_sub.SetName("t_phi_target")
       h_sim.SetName("t_phi_sim")

       h_tgt_sub.Add(h_dum, -1)

       for ii in range(1, h_tgt_sub.GetNbinsX()):

#           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
#              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

#           if(h_dum.GetBinContent(ii) == 0): 
#              sub_error = h_tgt.GetBinError(ii)
#           elif(h_tgt.GetBinContent(ii) == 0): 
#              sub_error = 1.0
#           else: 
#             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)

          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

          h_tgt_sub.SetBinError(ii, sub_error)
 


       c4.cd(i);

       h_sim.SetLineColor(2);
       h_dum.SetLineColor(3);

#       h_tgt.Draw();
#       h_dum.Draw("same")




#       h_tgt.Draw()


       h_tgt_sub.DrawCopy();
       h_temp = c4.cd(i).GetPrimitive("t_phi_target_copy")
       h_temp.SetName("t_phi_target")



       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

       print(i, " asdasdasdasd ")

    c4.Write(plot_name + "_t_phi_bin")





    # /*--------------------------------------------------*/
    # hsdelta in u-phi bin

    plot_name = "hsyptar"

    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_" + plot_name)
    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_" + plot_name)
    t_phi_sim = sim_omega_file.Get("t_phi_bin/t_phi_" + plot_name)

#    t_phi_tgt.Write("target_phi")

    c4 = TCanvas()
    c4.Divide(phi_bin_num, t_bin_num)

    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")

    for i in range(1, t_bin_num * phi_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_" + plot_name)
        h_tgt_sub = h_tgt.Clone("h_" + plot_name + "_sub")

       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_" + plot_name)
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_" + plot_name)

    
#        h_sim.SetName("h_miss_sim")
#        h_sim_rho.SetName("h_miss_sim_rho")
#        h_sim_xphsp.SetName("h_miss_sim_xphsp")
# 
       h_tgt.SetName("t_phi_target")
       h_tgt_sub.SetName("t_phi_target")
       h_sim.SetName("t_phi_sim")

       h_tgt_sub.Add(h_dum, -1)

       for ii in range(1, h_tgt_sub.GetNbinsX()):

#           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
#              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

#           if(h_dum.GetBinContent(ii) == 0): 
#              sub_error = h_tgt.GetBinError(ii)
#           elif(h_tgt.GetBinContent(ii) == 0): 
#              sub_error = 1.0
#           else: 
#             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)

          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )

          h_tgt_sub.SetBinError(ii, sub_error)
 


       c4.cd(i);

       h_sim.SetLineColor(2);
       h_dum.SetLineColor(3);

#       h_tgt.Draw();
#       h_dum.Draw("same")




#       h_tgt.Draw()


       h_tgt_sub.DrawCopy();
       h_temp = c4.cd(i).GetPrimitive("t_phi_target_copy")
       h_temp.SetName("t_phi_target")



       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

       print(i, " asdasdasdasd ")

    c4.Write(plot_name + "_t_phi_bin")



#
#
#    # /*--------------------------------------------------*/
#    # Q2 in u-phi bin
#
#    plot_name = "Q2"
#
#    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim = sim_omega_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_rho   = sim_rho_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_xphsp = sim_xphsp_file.Get("t_phi_bin/t_phi_" + plot_name)
#
#
#    t_phi_sim_eta  = sim_eta_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_etap = sim_etap_file.Get("t_phi_bin/t_phi_" + plot_name)
#
##    t_phi_tgt.Write("target_phi")
#
#    c4 = TCanvas()
#    c4.Divide(phi_bin_num, t_bin_num)
#
#    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")
#
#    for i in range(1, t_bin_num * phi_bin_num + 1):
#
#       # ttpad = t_phi_tgt.GetPad(i)
#
#        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_" + plot_name)
#        h_tgt_sub = h_tgt.Clone("h_" + plot_name + "_sub")
#
#       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_" + plot_name)
#
#       h_sim_rho = t_phi_sim_rho.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim_xphsp = t_phi_sim_xphsp.GetPad(i).GetPrimitive("h_" + plot_name)
#
#       h_sim_eta = t_phi_sim_eta.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim_etap = t_phi_sim_etap.GetPad(i).GetPrimitive("h_" + plot_name)
#
#    
#
#    
##        h_sim.SetName("h_miss_sim")
##        h_sim_rho.SetName("h_miss_sim_rho")
##        h_sim_xphsp.SetName("h_miss_sim_xphsp")
## 
#       h_tgt.SetName("t_phi_target")
#       h_tgt_sub.SetName("t_phi_target")
#       h_sim.SetName("t_phi_sim_omega")
#       h_sim_rho.SetName("t_phi_sim_rho")
#       h_sim_xphsp.SetName("t_phi_sim_xphsp")
#       h_sim_eta.SetName("t_phi_sim_eta")
#       h_sim_etap.SetName("t_phi_sim_etap")
#
#       h_tgt_sub.Add(h_dum, -1)
#
#       for ii in range(1, h_tgt_sub.GetNbinsX()):
#
##           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
##              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )
#
##           if(h_dum.GetBinContent(ii) == 0): 
##              sub_error = h_tgt.GetBinError(ii)
##           elif(h_tgt.GetBinContent(ii) == 0): 
##              sub_error = 1.0
##           else: 
##             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)
#
#          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )
#
#          h_tgt_sub.SetBinError(ii, sub_error)
# 
#
#
#       c4.cd(i);
#
#       h_sim.SetLineColor(2);
#       h_dum.SetLineColor(3);
#
##       h_tgt.Draw();
##       h_dum.Draw("same")
#
#
#
#
##       h_tgt.Draw()
#
#
#       h_tgt_sub.DrawCopy();
#
#       h_sim.Scale(omega_scale_factor)
#       h_sim_rho.Scale(rho_scale_factor)
#       h_sim_xphsp.Scale(xphsp_scale_factor)
#       h_sim_eta.Scale(eta_scale_factor)
#       h_sim_etap.Scale(etap_scale_factor)
#
#
#       h_sim.Draw("same")
#       h_sim_rho.Draw("same")
#       h_sim_xphsp.Draw("same")
#
#       h_sim_eta.Draw("same")
#       h_sim_etap.Draw("same")
#
#
#       print(i, " asdasdasdasd ")
#
#    c4.Write(plot_name + "_t_phi_bin")
#
#
#
#    # /*--------------------------------------------------*/
#    # W in u-phi bin
#
#    plot_name = "W"
#
#    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim = sim_omega_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_rho   = sim_rho_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_xphsp = sim_xphsp_file.Get("t_phi_bin/t_phi_" + plot_name)
#
#
#    t_phi_sim_eta  = sim_eta_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_etap = sim_etap_file.Get("t_phi_bin/t_phi_" + plot_name)
#
##    t_phi_tgt.Write("target_phi")
#
#    c4 = TCanvas()
#    c4.Divide(phi_bin_num, t_bin_num)
#
#    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")
#
#    for i in range(1, t_bin_num * phi_bin_num + 1):
#
#       # ttpad = t_phi_tgt.GetPad(i)
#
#        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_" + plot_name)
#        h_tgt_sub = h_tgt.Clone("h_" + plot_name + "_sub")
#
#       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_" + plot_name)
#
#       h_sim_rho = t_phi_sim_rho.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim_xphsp = t_phi_sim_xphsp.GetPad(i).GetPrimitive("h_" + plot_name)
#
#       h_sim_eta = t_phi_sim_eta.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim_etap = t_phi_sim_etap.GetPad(i).GetPrimitive("h_" + plot_name)
#
#    
#
#    
##        h_sim.SetName("h_miss_sim")
##        h_sim_rho.SetName("h_miss_sim_rho")
##        h_sim_xphsp.SetName("h_miss_sim_xphsp")
## 
#       h_tgt.SetName("t_phi_target")
#       h_tgt_sub.SetName("t_phi_target")
#       h_sim.SetName("t_phi_sim_omega")
#       h_sim_rho.SetName("t_phi_sim_rho")
#       h_sim_xphsp.SetName("t_phi_sim_xphsp")
#       h_sim_eta.SetName("t_phi_sim_eta")
#       h_sim_etap.SetName("t_phi_sim_etap")
#
#       h_tgt_sub.Add(h_dum, -1)
#
#       for ii in range(1, h_tgt_sub.GetNbinsX()):
#
##           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
##              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )
#
##           if(h_dum.GetBinContent(ii) == 0): 
##              sub_error = h_tgt.GetBinError(ii)
##           elif(h_tgt.GetBinContent(ii) == 0): 
##              sub_error = 1.0
##           else: 
##             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)
#
#          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )
#
#          h_tgt_sub.SetBinError(ii, sub_error)
# 
#
#
#       c4.cd(i);
#
#       h_sim.SetLineColor(2);
#       h_dum.SetLineColor(3);
#
##       h_tgt.Draw();
##       h_dum.Draw("same")
#
#
#
#
##       h_tgt.Draw()
#
#
#       h_tgt_sub.DrawCopy();
#
#       h_sim.Scale(omega_scale_factor)
#       h_sim_rho.Scale(rho_scale_factor)
#       h_sim_xphsp.Scale(xphsp_scale_factor)
#       h_sim_eta.Scale(eta_scale_factor)
#       h_sim_etap.Scale(etap_scale_factor)
#
#
#       h_sim.Draw("same")
#       h_sim_rho.Draw("same")
#       h_sim_xphsp.Draw("same")
#
#       h_sim_eta.Draw("same")
#       h_sim_etap.Draw("same")
#
#
#       print(i, " asdasdasdasd ")
#
#    c4.Write(plot_name + "_t_phi_bin")
#
#
#
#
#
#    # /*--------------------------------------------------*/
#    # u in u-phi bin
#
#    plot_name = "u"
#
#    t_phi_tgt = tgt_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_dum = dum_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim = sim_omega_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_rho   = sim_rho_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_xphsp = sim_xphsp_file.Get("t_phi_bin/t_phi_" + plot_name)
#
#
#    t_phi_sim_eta  = sim_eta_file.Get("t_phi_bin/t_phi_" + plot_name)
#    t_phi_sim_etap = sim_etap_file.Get("t_phi_bin/t_phi_" + plot_name)
#
##    t_phi_tgt.Write("target_phi")
#
#    c4 = TCanvas()
#    c4.Divide(phi_bin_num, t_bin_num)
#
#    print("asdasasdasd   ", t_phi_tgt.GetNumber(), "        sadfsadf ")
#
#    for i in range(1, t_bin_num * phi_bin_num + 1):
#
#       # ttpad = t_phi_tgt.GetPad(i)
#
#        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("h_" + plot_name)
#        h_tgt_sub = h_tgt.Clone("h_" + plot_name + "_sub")
#
#       h_dum = t_phi_dum.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim = t_phi_sim.GetPad(i).GetPrimitive("h_" + plot_name)
#
#       h_sim_rho = t_phi_sim_rho.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim_xphsp = t_phi_sim_xphsp.GetPad(i).GetPrimitive("h_" + plot_name)
#
#       h_sim_eta = t_phi_sim_eta.GetPad(i).GetPrimitive("h_" + plot_name)
#       h_sim_etap = t_phi_sim_etap.GetPad(i).GetPrimitive("h_" + plot_name)
#
#    
#
#    
##        h_sim.SetName("h_miss_sim")
##        h_sim_rho.SetName("h_miss_sim_rho")
##        h_sim_xphsp.SetName("h_miss_sim_xphsp")
## 
#       h_tgt.SetName("t_phi_target")
#       h_tgt_sub.SetName("t_phi_target")
#       h_sim.SetName("t_phi_sim_omega")
#       h_sim_rho.SetName("t_phi_sim_rho")
#       h_sim_xphsp.SetName("t_phi_sim_xphsp")
#       h_sim_eta.SetName("t_phi_sim_eta")
#       h_sim_etap.SetName("t_phi_sim_etap")
#
#       h_tgt_sub.Add(h_dum, -1)
#
#       for ii in range(1, h_tgt_sub.GetNbinsX()):
#
##           if(h_tgt.GetBinContent(ii)== 0 or h_dum.GetBinContent(ii) == 0): 
##              sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )
#
##           if(h_dum.GetBinContent(ii) == 0): 
##              sub_error = h_tgt.GetBinError(ii)
##           elif(h_tgt.GetBinContent(ii) == 0): 
##              sub_error = 1.0
##           else: 
##             sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2) * h_tgt_sub.GetBinContent(ii)
#
#          sub_error = sqrt( h_tgt.GetBinError(ii)**2 + h_dum.GetBinError(ii)**2 )
#
#          h_tgt_sub.SetBinError(ii, sub_error)
# 
#
#
#       c4.cd(i);
#
#       h_sim.SetLineColor(2);
#       h_dum.SetLineColor(3);
#
##       h_tgt.Draw();
##       h_dum.Draw("same")
#
#
#
#
##       h_tgt.Draw()
#
#
#       h_tgt_sub.DrawCopy();
#
#       h_sim.Scale(omega_scale_factor)
#       h_sim_rho.Scale(rho_scale_factor)
#       h_sim_xphsp.Scale(xphsp_scale_factor)
#       h_sim_eta.Scale(eta_scale_factor)
#       h_sim_etap.Scale(etap_scale_factor)
#
#
#       h_sim.Draw("same")
#       h_sim_rho.Draw("same")
#       h_sim_xphsp.Draw("same")
#
#       h_sim_eta.Draw("same")
#       h_sim_etap.Draw("same")
#
#
#       print(i, " asdasdasdasd ")
#
#    c4.Write(plot_name + "_t_phi_bin")
#
#




























    # /*--------------------------------------------------*/
    # Missing mass in t bin

    t_phi_tgt = tgt_file.Get("t_phi_bin/missmass_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/missmass_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/missmass_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("missmass_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive("missmass_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive("missmass_real_var")

#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName("missmass_t_sim")
#        h_sim_rho.SetName("missmass_t_sim_rho")
#        h_sim_xphsp.SetName("missmass_t_sim_xphsp")


       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")


    c5.Write("missmass_t_bin")



# 
# 
# 
# 
#    # /*--------------------------------------------------*/
#    # W in t bin
# 
#    t_phi_tgt = tgt_file.Get("t_phi_bin/W_real_sub")
#    t_phi_dum = dum_file.Get("t_phi_bin/W_real_sub")
#    t_phi_sim = sim_omega_file.Get("t_phi_bin/W_real_sub")
#    t_phi_sim_rho = sim_rho_file.Get("t_phi_bin/W_real_sub")
#    t_phi_sim_xphsp = sim_xphsp_file.Get("t_phi_bin/W_real_sub")
# 
#    c5 = TCanvas()
#    c5.Divide(3,1)
# 
#    
#    
#    for i in range(1, t_bin_num + 1):
# 
#        # ttpad = t_phi_tgt.GetPad(i)
# 
#        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive("W_real_var")
#        h_dum = t_phi_dum.GetPad(i).GetPrimitive("W_real_var")
#        h_sim = t_phi_sim.GetPad(i).GetPrimitive("W_real_var")
#        h_sim_rho = t_phi_sim_rho.GetPad(i).GetPrimitive("W_real_var")
#        h_sim_xphsp = t_phi_sim_xphsp.GetPad(i).GetPrimitive("W_real_var")
#    
#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName("W_t_sim")
#        h_sim_rho.SetName("W_t_sim_rho")
#        h_sim_xphsp.SetName("W_t_sim_xphsp")
# 
#        h_sim.SetLineColor(2)
#        h_dum.SetLineColor(3)
# 
# #       h_tgt.Draw();
# #       h_dum.Draw("same");
# 
#        h_tgt.Add(h_dum, -1)
# 
#        c5.cd(i);
# 
#        h_tgt.Draw()
# 
#        h_sim.Scale(omega_scale_factor)
#        h_sim_rho.Scale(rho_scale_factor)
#        h_sim_xphsp.Scale(xphsp_scale_factor)
# 
#        h_sim.Draw("same")
#        h_sim_rho.Draw("same")
#        h_sim_xphsp.Draw("same")
# 
# 
# 
# 
#    c5.Write("t_bin_W")


    # /*--------------------------------------------------*/
    # W in t bin

    plot_str = "W"

    t_phi_tgt = tgt_file.Get("t_phi_bin/" + plot_str +"_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/" + plot_str   + "_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/" + plot_str  + "_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive(plot_str + "_real_var")

#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName(plot_str + "_t_sim")
#        h_sim_rho.SetName(plot_str + "_t_sim_rho")
#        h_sim_xphsp.SetName(plot_str + "_t_sim_xphsp")

       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

    c5.Write(plot_str + "_t_bin")




    # /*--------------------------------------------------*/
    # Q2 in t bin

    plot_str = "Q2"

    t_phi_tgt = tgt_file.Get("t_phi_bin/" + plot_str +"_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/" + plot_str   + "_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/" + plot_str  + "_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    print("asdasdasdadsa")
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive(plot_str + "_real_var")

       print("111111111111    asdasdasdadsa")
    
#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName(plot_str + "_t_sim")
#        h_sim_rho.SetName(plot_str + "_t_sim_rho")
#        h_sim_xphsp.SetName(plot_str + "_t_sim_xphsp")

       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

    c5.Write(plot_str + "_t_bin")




    # /*--------------------------------------------------*/
    # u in t bin

    plot_str = "u"

    t_phi_tgt = tgt_file.Get("t_phi_bin/" + plot_str +"_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/" + plot_str   + "_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/" + plot_str  + "_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive(plot_str + "_real_var")
    
#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName(plot_str + "_t_sim")
#        h_sim_rho.SetName(plot_str + "_t_sim_rho")
#        h_sim_xphsp.SetName(plot_str + "_t_sim_xphsp")

       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

    c5.Write(plot_str + "_t_bin")



    # /*--------------------------------------------------*/
    # t in u bin

    plot_str = "t"

    t_phi_tgt = tgt_file.Get("t_phi_bin/" + plot_str +"_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/" + plot_str   + "_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/" + plot_str  + "_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive(plot_str + "_real_var")
    
#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName(plot_str + "_t_sim")
#        h_sim_rho.SetName(plot_str + "_t_sim_rho")
#        h_sim_xphsp.SetName(plot_str + "_t_sim_xphsp")

       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

    c5.Write(plot_str + "_t_bin")



    # /*--------------------------------------------------*/
    # Em in u bin

    plot_str = "Em"

    t_phi_tgt = tgt_file.Get("t_phi_bin/" + plot_str +"_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/" + plot_str   + "_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/" + plot_str  + "_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive(plot_str + "_real_var")
    
#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName(plot_str + "_t_sim")
#        h_sim_rho.SetName(plot_str + "_t_sim_rho")
#        h_sim_xphsp.SetName(plot_str + "_t_sim_xphsp")

       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

    c5.Write(plot_str + "_t_bin")



    # /*--------------------------------------------------*/
    # t in u bin

    plot_str = "Pm"

    t_phi_tgt = tgt_file.Get("t_phi_bin/" + plot_str +"_real_sub")
    t_phi_dum = dum_file.Get("t_phi_bin/" + plot_str   + "_real_sub")
    t_phi_sim = sim_file.Get("t_phi_bin/" + plot_str  + "_real_sub")

    c5 = TCanvas()
    c5.Divide(t_bin_num,1)

    
    
    for i in range(1, t_bin_num + 1):

       # ttpad = t_phi_tgt.GetPad(i)

        h_tgt = t_phi_tgt.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_dum = t_phi_dum.GetPad(i).GetPrimitive(plot_str + "_real_var")
       h_sim = t_phi_sim.GetPad(i).GetPrimitive(plot_str + "_real_var")
    
#        # h_tgt.SetName("missmass_t_real")
#        h_sim.SetName(plot_str + "_t_sim")
#        h_sim_rho.SetName(plot_str + "_t_sim_rho")
#        h_sim_xphsp.SetName(plot_str + "_t_sim_xphsp")

       h_tgt.SetName("t_target")
       h_sim.SetName("t_sim")

       h_sim.SetLineColor(2)
       h_dum.SetLineColor(3)

#       h_tgt.Draw();
#       h_dum.Draw("same");

       h_tgt.Add(h_dum, -1)

       c5.cd(i);

       h_tgt.Draw()

       h_sim.Scale(scale_factor)

       h_sim.Draw("same")

    c5.Write(plot_str + "_t_bin")








    file_out.Close

    return



# /*--------------------------------------------------*/
# /*--------------------------------------------------*/
# /*--------------------------------------------------*/

def Fit_missmass(missmass_sim, missmass_tot):
    

    peak = missmass_sim

    c4 = TCanvas() 

    f1 = TF1("f1", ftotal, 0.55, 1, 5);

    f1.SetParameters(6.3, -31, 48, -23, 70)
    f1.SetParLimits(4, 65, 80)


#    missmass_sub = missmass_tot.Clone("new")

    missmass_tot.Draw();

# 
#    missmass_sub.Add(missmass_sim, -1)
# 
#    missmass_sub.SetLineColor(1)
# 

 #    missmass_sub.Draw("HISTsame")


    missmass_tot.Fit("f1", "R"); 


#    missmass_sim.Draw("same")

    peak.Draw("same")

#    c4.Write("missassaaaaaaaaa")
# 
#    c4.Print("missmass_fit.pdf"))
# 
# 
# 
#    c5 = TCanvas() 
# 
#    missmass_sub.SetOption("E")
#     missmass_sub.Draw()
# 

#    f1.Draw("same")


    c4.Print("fit.pdf"))

    exit(1)


    return












# /*--------------------------------------------------*/
# /*--------------------------------------------------*/
# /*--------------------------------------------------*/
## Fitting Function Definition


def ftotal(x, par):

   xx = x[0]

   bin = peak.GetXaxis().FindBin(xx)
   sr = par[4] * peak.GetBinContent(bin)
   br = par[0]  + par[1] * xx  + par[2]* xx * xx + par[3]* xx * xx *  xx
    
   return sr + br;

#    return br;
















#file_list = ["160"]

#for setting in file_list:
#    Setting_by_setting(setting)



q2_setting    = ["160", "245"]

#q2_setting    = ["160"]

eps_160       = ["32",  "59"]
#eps_160       = ["32"]

eps_245       = ["27", "55"]

hms_angle_160_l  = ['+0970',  '+3000']
hms_angle_160_h  = ['-2730',  '+0000', '+3000']

hms_angle_245_l  = ['+1350',  '+3000']
hms_angle_245_h  = ['-3000',  '+0000', '+3000']

for setting in q2_setting:


    if float(setting) == 160:
       for eps in eps_160:
          if float(eps) == 32:
             for angle in hms_angle_160_l:
                print(setting + "_"+ eps  + "_" + angle)
                Setting_by_setting(setting + "_"+ eps  + "_" + angle)

                #exit(0);

          else:
             for angle in hms_angle_160_h:
                print(setting + "_"+ eps  + "_" + angle)
                Setting_by_setting(setting + "_"+ eps  + "_" + angle)
       


    else:       
       for eps in eps_245:
          if float(eps) == 27:
             for angle in hms_angle_245_l:
                print(setting + "_"+ eps  + "_" + angle)
                Setting_by_setting(setting + "_"+ eps  + "_" + angle)
          else:
             for angle in hms_angle_245_h:
                print(setting + "_"+ eps  + "_" + angle)
                Setting_by_setting(setting + "_"+ eps  + "_" + angle)






#                Setting_by_setting(setting + "_"+ eps  + "_" + angle)

       

#          for angle in hms_angle_160_l:
#          print(setting + "_"+ eps  + "_" + angle)
#             Setting_by_setting(setting + "_"+ eps  + "_" + angle)












# 
# # 
# # dum_file = TFile("yields.heep_191_27_0000.dummy.root", "READ")
# # sim_file = TFile("yields.heep_191_27_0000.sim.root", "READ")
# # tgt_file = TFile("yields.heep_191_27_0000.target.root", "READ")
# # 
# 
# 
# # 
# # dum_file = TFile("yields.heep_442_27_0000.dummy.root", "READ")
# # sim_file = TFile("yields.heep_442_27_0000.sim.root", "READ")
# # tgt_file = TFile("yields.heep_442_27_0000.target.root", "READ")
# # 
# 
# # 
# # dum_file = TFile("yields.heep_542_27_0000.dummy.root", "READ")
# # sim_file = TFile("yields.heep_542_27_0000.sim.root", "READ")
# # tgt_file = TFile("yields.heep_542_27_0000.target.root", "READ")
# # 
# 
# 
# dum_file = TFile("yields.heep_653_27_0000.dummy.root", "READ")
# sim_file = TFile("yields.heep_653_27_0000.sim.root", "READ")
# tgt_file = TFile("yields.heep_653_27_0000.target.root", "READ")
# 
# 
# 
# 
# 
# 
# 
# # 
# # sim_file.ls()
# # dum_file.ls()
# # tgt_file.ls()
# # 
# 
# 
# 
# #plot_name = "missmass"
# #plot_name = "hsdelta"
# #plot_name = "q2"
# #plot_name = "w"
# #plot_name = "hsxptar"
# #plot_name = "hsyptar"
# #plot_name = "hsytar"
# #plot_name = "t"
# 
# 
# #plot_name = "ssytar"
# #plot_name = "ssyptar"
# #plot_name = "ssxptar"
# 
# #plot_name = "th_pq"
# #plot_name = "phi_pq"
# 
# #plot_name = "pmpar"
# #plot_name = "pmper"
# #plot_name = "pmoop"    
# 
# plot_name_list = ["missmass", "hsdelta", "q2", "w", "hsxptar", "hsyptar", "hsytar", "t", "ssytar", "ssyptar", "ssxptar", "th_pq", "phi_pq", "pmpar", "pmper", "pmoop"]
# 
# print(plot_name_list[0]   )
# 
# 
# plot_name = plot_name_list[0]
# 
# 
# missmass=[]
# 
# missmass_tmp = tgt_file.Get(plot_name)
# missmass.append(missmass_tmp)
# 
# missmass_tmp = dum_file.Get(plot_name)
# missmass.append(missmass_tmp)
# 
# missmass_tmp = sim_file.Get(plot_name)
# missmass.append(missmass_tmp)
# 
# 
# 
# 
# 
# # missmass[2].SetLineColor(2)
# # missmass[2].Draw()
# # 
# # missmass[0].Draw("same")
# 
# 
# 
# 
# file_out = TFile("out_put.root", "RECREATE")
# 
# 
# for x in range(len(plot_name_list)):
# 
#    missmass[:] = []
# 
#    print(x, plot_name_list[x])
#    plot_name = plot_name_list[x]
# 
#    missmass_tmp = tgt_file.Get(plot_name)
#    missmass.append(missmass_tmp)
# #    
#     missmass_tmp = dum_file.Get(plot_name)
#     missmass.append(missmass_tmp)
# #    
# 
# 
#     missmass_tmp = sim_file.Get(plot_name)
#     missmass.append(missmass_tmp)
# #
# 
# 
#  
# #    missmass[0].Draw()
# #    missmass[1].Draw("same")
# #    missmass[2].Draw("same")
# 
# 
# #    missmass[0].Add(missmass[1], -1)
# 
#    c1 = TCanvas() 
# 
#    if(missmass[0].GetMaximum() > missmass[2].GetMaximum()):
#        missmass[2].SetMaximum(missmass[0].GetMaximum() + 0.2*missmass[0].GetMaximum());
# 
# 
#    missmass[2].SetLineColor(2)
#     missmass[2].Draw()
# 
#    missmass[1].SetLineColor(3)
#     missmass[1].Draw("same")
# 
#     missmass[0].Draw("same")
# 
# 
#    file_out.cd()
#    c1.Write(plot_name)
# 
# 
#    
# 
# 
# # #missmass[2].Draw()
# 
# #print(missmass[0].GetSum(), missmass[2].GetSum())
# #print(missmass[0].GetBinWidth(3), missmass[2].GetBinWidth(3)  )
# 
# 


# def func_advanced (x, p) :
#    if (x[0] < 1.25) :
#        return p[0] + p[1]*x[0] + p[2]*x[0]*x[0]
#    else :
#        return p[0] + p[1]*x[0] + 0.9*x[0]*x[0]




# 
# Double_t ftotal(Double_t *x, Double_t *par) {
#    Double_t xx = x[0];
#    Int_t bin = background->GetXaxis()->FindBin(xx);
#    Double_t br = par[3]*background->GetBinContent(bin);
#    Double_t arg = (xx-par[1])/par[2];
#    Double_t sr = par[0]*TMath::Exp(-0.5*arg*arg);
#    return sr + br;
# }
#



# Double_t ftotal(Double_t *x, Double_t *par) {
#    Double_t xx = x[0];
#    Int_t bin = background->GetXaxis()->FindBin(xx);
#    Double_t br = par[3]*background->GetBinContent(bin);
#    Double_t arg = (xx-par[1])/par[2];
#    Double_t sr = par[0]*TMath::Exp(-0.5*arg*arg);
#    return sr + br;
# }
# 
# def ftotal(x, par) {
#    Double_t xx = x[0];
#    Int_t bin = background->GetXaxis()->FindBin(xx);
#    Double_t br = par[3]*background->GetBinContent(bin);
#    Double_t arg = (xx-par[1])/par[2];
#    Double_t sr = par[0]*TMath::Exp(-0.5*arg*arg);
#    return sr + br;
# }
# 
# 






