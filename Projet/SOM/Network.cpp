#include "Network.h"

namespace SOM
{
	Network* Network::instance = nullptr;

	Network* Network::GetInstance(Settings &settings)
	{
		if (instance == nullptr)
			instance = new Network(settings);
		
		return instance;
	}

	//Constructeur
	Network::Network(Settings &settings): m_vWinner(settings.m_nNetworkDim)
	{
		m_settings = settings;

		//initialisation alpha et beta
		m_fAlpha = settings.m_dInitialAlpha;
		m_fBeta = settings.m_nInitialBeta;

		uint m_nCurrentIteration = 1;
		m_nNbIterationMax = 0;

		//Création du vecteur de neurones
		m_vvNetwork.resize(m_settings.m_nNbCols);
		for (int i = 0; i < m_settings.m_nNbCols; ++i)
			m_vvNetwork[i].resize(m_settings.m_nNbRows);

		//Initialisation du vecteur de neurones
		for(uint rows=0;rows<m_settings.m_nNbRows;++rows)
			for (uint cols = 0; cols < m_settings.m_nNbCols; ++cols)
			{
				Neuron m_NNeuron(m_settings.m_nDimInputVector);
				m_vvNetwork[cols][rows] = m_NNeuron;
			}
	}

	void Network::calcNbMaxIterations()
	{
		uint iteration = 1;
		while (m_fAlpha > 0.000000001)
		{
			m_fAlpha = m_settings.m_dInitialAlpha * exp(-(double)iteration / m_settings.m_dAlphaRate);
			m_nNbIterationMax++;
			iteration++;
		}
		m_nNbIterationMax *= m_settings.m_nAlphaPeriod;
	}

	void Network::UpdateAlpha()
	{
		if (m_nCurrentIteration % m_settings.m_nAlphaPeriod == 0)
			m_fAlpha = m_settings.m_dInitialAlpha * exp(m_nCurrentIteration / -m_settings.m_dAlphaRate);
	}

	void Network::UpdateBeta()
	{
		if (m_nCurrentIteration % m_settings.m_nBetaPeriod == 0)
			m_fBeta = m_settings.m_nInitialBeta * exp(m_nCurrentIteration / -m_settings.m_dBetaRate);
	}

	double Network::GetActivity(Vector coordinate)
	{
		auto distanceType = DistanceMetric::EUCL;
		double activity = 0;
		switch(distanceType)
				{
					case EUCL:
						for (uint idWeight = 0; idWeight < m_nDimInputVector; ++idWeight) 
							activity += pow((m_fInput[idWeight] - m_vvNetwork[coordinate[0]][coordinate[1]].GetWeight(idWeight)), 2);
						activity = sqrt(activity);
				}
		return activity;
	}
	double Network::GetDistance(Vector coordinate)
	{
		auto distanceType = DistanceMetric::EUCL;
		double distance = 0;
		switch (distanceType)
		{
		case EUCL:
			for (uint idWeight = 0; idWeight < m_nDimInputVector; ++idWeight)
				distance += pow((m_vvNetwork[m_vWinner[0]][m_vWinner[1]].GetWeight(idWeight) - m_vvNetwork[coordinate[0]][coordinate[1]].GetWeight(idWeight)), 2);
			distance = sqrt(distance);
		}
		return distance;
	}

	void Network::AlgoSOM(int currentIteration)
	{		
			//TODO: Implémenter l'algorithme de SOM
			m_fAlpha = m_settings.m_dInitialAlpha * exp(-currentIteration / m_settings.m_dAlphaRate);;
			m_fBeta--;		
	}

	// TODO: Implémenter UpdateNeighbour dans l'algorithme de SOM
	void Network::UpdateNeighbour()
	{
		Vector vNeuron(2);
		for (uint row = 0; row < m_settings.m_nNbRows; ++row)
			for (uint col = 0; col < m_settings.m_nNbCols; ++col)
			{
				vNeuron[0] = row;
				vNeuron[1] = col;
				m_fPhi = exp(-GetDistance(vNeuron) / 2 * m_fBeta);
			}
	}
	
	void Network::SetWinner()
	{
		m_fMinAct = 66000;
		double activity;
		Vector vNeuron(2);
		for (uint row = 0; row < m_settings.m_nNbRows; ++row)
			for (uint col = 0; col < m_settings.m_nNbCols; ++col)
			{
				// Neurone pour lequel on calcule l'activité
				vNeuron[0] = row;
				vNeuron[1] = col;

				//Calcul de l'activité
				activity = GetActivity(vNeuron); 

				// Verification si vNeuron est le neurone vainqueur
				if (activity < m_fMinAct)
				{
					m_fMinAct = activity;
					//Modification du neurone vainqueur
					m_vWinner = vNeuron;
				}
			}
	}

	//TODO:Implémenter UpdateWeight dans l'algorithme de SOM
	void Network::UpdateWeight(double* weight)
	{
		for (uint row = 0; row < m_settings.m_nNbRows; ++row)
			for (uint col = 0; col < m_settings.m_nNbCols; ++col)
				for (uint idWeight = 0; idWeight < m_nDimInputVector; ++idWeight)
					m_vvNetwork[row][col].SetWeight(idWeight, m_fAlpha, m_fPhi, m_fInput);
	}

	Neuron& Network::getNeuron(int row, int col)
	{
		return m_vvNetwork[col][row];
	}
}