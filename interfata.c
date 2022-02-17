#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "interfata.h"

//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1


//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
int intervals=20;
double axis[100];
int hist[100];
double *filterTime=0;
double *filterTime2=0;
int dimFereastra;
int filterType;
double filterType2;
int stopVar=0;
int startVar=0;
double alpha=0.0;

int N=0;
WindowConst winConst;
double dt;

static int acqPanel;
static int mainPanel;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "interfata.uir", MAIN_PANEL)) < 0)
		return -1;
	if ((acqPanel = LoadPanel (0, "interfata.uir", FREQ_PANEL)) < 0)
		return -1;
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK OnMainPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnFreqPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	double maxVal = 0.0;
	double minVal = 0.0;
	
	double mean = 0.0;
	
	int maxIndex = 0;
	int minIndex = 0;
	double median =0.0;
	
	double dispersion = 0.0;  
	
	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			PlotY(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			
			
			
			//calculare minim, maxim, mean, mediana, dispersie
			MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);//1
			
			Mean(waveData,npoints,&mean);								   //2
			
			Median(waveData,npoints,&median);							   //3
			
			StdDev(waveData,npoints,&mean,&dispersion);						//4
			
			Histogram(waveData,npoints,minVal-1,maxVal+1,hist,axis,intervals);
			DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_3, -1, VAL_DELAYED_DRAW);
            PlotXY (panel, MAIN_PANEL_GRAPH_3, axis,  hist, intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
			
			//zero crossing
			int zero = 0;
			for ( int i = 2; i < npoints; i++){
				if((waveData[i] * waveData[i-1]) < 0){
					zero++;
				}
			}
			
			SetCtrlVal(panel, MAIN_PANEL_MAX_BTN, maxVal);				   //1
			SetCtrlVal(panel, MAIN_PANEL_MIN_BTN, minVal);				   //1
			
			SetCtrlVal(panel, MAIN_PANEL_MEAN_BTN, mean);				   //2
			
			SetCtrlVal(panel, MAIN_PANEL_MAX_INDEX_BTN, (double)minIndex); //3
			SetCtrlVal(panel, MAIN_PANEL_MIN_INDEX_BTN, (double)maxIndex); //3
			SetCtrlVal(panel, MAIN_PANEL_MEDIAN_BTN, median);			   //3
			
			SetCtrlVal(panel, MAIN_PANEL_DISP_BTN, dispersion);			   //4
			
			SetCtrlVal(panel, MAIN_PANEL_NPOINTS_BTN, npoints);
			SetCtrlVal(panel, MAIN_PANEL_SAMPLE_BTN, sampleRate);
			SetCtrlVal(panel, MAIN_PANEL_ZERO_BTN, zero);
			
			break;
	}
	return 0;
}

int CVICALLBACK OnAplicaButton (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(mainPanel, MAIN_PANEL_RING, &filterType);
			filterTime = (double *) calloc(npoints+5000, sizeof(double));

			if(filterType== 0) // filtru mediere
			{	
				double s = 0;
				
				GetCtrlVal(mainPanel, MAIN_PANEL_DIM_FER, &dimFereastra);
				
				for (int i = 0; i < npoints; i++)
				{
					for (int k = 0; k <= dimFereastra - 1; k++)
					{
						if ((i - k) < 0)
						{
							s += 0;
						}
						else	
						{
							s += waveData[i-k];	
						}
					}
					filterTime[i] = s/dimFereastra;
					s = 0;
					
				}
			}
			else if(filterType == 1) // filtru element de ordin 1
			{	
				GetCtrlVal(mainPanel, MAIN_PANEL_ALPHA, &alpha);
				filterTime[0] = waveData[0];
				
				for(int i = 1; i < npoints; i++)
				{
					filterTime[i] = (1-alpha)*filterTime[i-1] + alpha*waveData[i];
				}
			}
			// extrag secundele 
			GetCtrlVal(mainPanel, MAIN_PANEL_STOP, &stopVar);
			GetCtrlVal(mainPanel, MAIN_PANEL_START, &startVar);
			double *p = filterTime + (int)(startVar * sampleRate);
			
			DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
			PlotY(panel, MAIN_PANEL_GRAPH_2, p,(stopVar - startVar)*sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			break;
	}
	return 0;
}

int CVICALLBACK OnNextButton (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int val1 =(int) npoints/sampleRate;
			int valStop = 0;
			int valStart = 0;
			
			GetCtrlVal(mainPanel, MAIN_PANEL_START, &valStart);
			GetCtrlVal(mainPanel, MAIN_PANEL_STOP, &valStop);
			if(valStart >= 0 && valStop < val1)
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_START, valStart+1);
				SetCtrlVal(mainPanel, MAIN_PANEL_STOP, valStop+1);
				OnAplicaButton(mainPanel,MAIN_PANEL_NEXT_BUTTON, EVENT_COMMIT,0,0,0);
			}		
			
			break;
	}
	return 0;
}

int CVICALLBACK OnPrevButton (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int val1 =(int) npoints/sampleRate;
			int valStop = 0;
			int valStart = 0;
			
			GetCtrlVal(mainPanel, MAIN_PANEL_START, &valStart);
			GetCtrlVal(mainPanel, MAIN_PANEL_STOP, &valStop);
			if(valStart > 0 && valStop <= val1)
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_START, valStart-1);
				SetCtrlVal(mainPanel, MAIN_PANEL_STOP, valStop-1);
				OnAplicaButton(mainPanel,MAIN_PANEL_PREV_BUTTON, EVENT_COMMIT,0,0,0);
			}		
			
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel)
			{
				SetCtrlVal(acqPanel, FREQ_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(acqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(mainPanel, FREQ_PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnFrqButton(int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	GetCtrlVal(panel, FREQ_PANEL_NR_PUNCTE, &N); 
	double temp[N];
	double autoSpectrum[N/2];
	double dt = 1.0/sampleRate;
	double convertedSpectrum[N/2]; 
	double powerPeak = 0.0;
	double freqPeak = 0.0;  
	double df = 0.0;
	int i;
	char unit[32] = "V";
	int tipFer;
													 
	
	switch (event)
	{
		case EVENT_COMMIT:

			for(i=0; i<N;i++)
			{
				temp[i] = waveData[i];
			}
			GetCtrlVal(panel,FREQ_PANEL_RING_TIP_FEREASTRA, &tipFer);
			if(tipFer==0)
			{
				//aplatizare forma semnal la capetele intervalului
				ScaledWindow(temp,N, WELCH, &winConst);
			}
			if(tipFer==1)
			{
				//aplatizare forma semnal la capetele intervalului
				ScaledWindow(temp,N, FLATTOP, &winConst);
			}
			//partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
			AutoPowerSpectrum(temp,N, dt, autoSpectrum, &df); 
			//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului  
			PowerFrequencyEstimate(autoSpectrum, N/2, -1.0, winConst, df,7, &freqPeak,&powerPeak);
			SpectrumUnitConversion(autoSpectrum,N/2, 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum,unit);
			//afisez pe interfata valorile determinate				    
			SetCtrlVal( acqPanel, FREQ_PANEL_IDC_FREQ_PEAK, freqPeak);
			SetCtrlVal(acqPanel, FREQ_PANEL_IDC_POWER_PEAK, powerPeak);
			//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
			//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			SpectrumUnitConversion(autoSpectrum,N/2,SPECTRUM_POWER,SCALING_MODE_LINEAR
						   ,DISPLAY_UNIT_VRMS,df,winConst,convertedSpectrum,unit);
			//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (acqPanel, FREQ_PANEL_IDC_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);	
			//plotez spectrul semnalului
		    PlotWaveform( acqPanel, FREQ_PANEL_IDC_GRAPH_SPECTRU, convertedSpectrum, N/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
						  VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			break;
	}
	return 0;
}
int CVICALLBACK OnFilterButton(int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	GetCtrlVal(panel, FREQ_PANEL_NR_PUNCTE, &N); 
	double temp[N];
	double autoSpectrum[N/2];
	double dt = 1.0/sampleRate;
	double convertedSpectrum[N/2]; 
	double powerPeak = 0.0;
	double freqPeak = 0.0;  
	double df = 0.0;
	int i;
	char unit[32] = "V";
	int tipFer;
	int filtru=0;
	
	switch(event)
	{
		case EVENT_COMMIT:
			filterTime2=(double*) calloc(npoints, sizeof(double));
			GetCtrlVal(acqPanel, FREQ_PANEL_RING_TIP_FILTRU, &filtru);
			if(filtru== 0) // filtru mediere
			{	
				double s = 0;
				
				for (int i = 0; i < N; i++)
				{
					for (int k = 0; k <= 32 - 1; k++)
					{
						if ((i - k) < 0)
						{
							s += 0;
						}
						else	
						{
							s += waveData[i-k];	
						}
					}
					filterTime2[i] = s/32;
					s = 0;
					
				}
			}
			else if(filtru==1)
			{
				Ch_HPF(waveData,N,sampleRate,900,0.1,6,filterTime2);
			}
			for(i=0; i<N;i++)
			{
				temp[i] = filterTime2[i];
			}
			GetCtrlVal(panel,FREQ_PANEL_RING_TIP_FEREASTRA, &tipFer);
			if(tipFer==0)
			{
				//aplatizare forma semnal la capetele intervalului
				ScaledWindow(temp,N, WELCH, &winConst);
			}
			if(tipFer==1)
			{
				//aplatizare forma semnal la capetele intervalului
				ScaledWindow(temp,N, FLATTOP, &winConst);
			}
			//partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
			AutoPowerSpectrum(temp,N, dt, autoSpectrum, &df); 
			//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului  
			PowerFrequencyEstimate(autoSpectrum, N/2, -1.0, winConst, df,7, &freqPeak,&powerPeak);
			SpectrumUnitConversion(autoSpectrum,N/2, 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum,unit);
			//afisez pe interfata valorile determinate				    
			//SetCtrlVal( acqPanel, FREQ_PANEL_IDC_FREQ_PEAK, freqPeak);
			//SetCtrlVal(acqPanel, FREQ_PANEL_IDC_POWER_PEAK, powerPeak);
			//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
			//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			SpectrumUnitConversion(autoSpectrum,N/2,SPECTRUM_POWER,SCALING_MODE_LINEAR
						   ,DISPLAY_UNIT_VRMS,df,winConst,convertedSpectrum,unit);
			//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (acqPanel, FREQ_PANEL_GRAPH_SPECTRU_FILTRAT, -1, VAL_IMMEDIATE_DRAW);	
			//plotez spectrul semnalului
		    PlotWaveform( acqPanel, FREQ_PANEL_GRAPH_SPECTRU_FILTRAT, convertedSpectrum, N/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
						  VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			break;
			
	}
	return 0;
}
