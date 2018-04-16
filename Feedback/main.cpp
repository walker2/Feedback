#include <iostream>
#include <random>

float AvgInWaitingAlgorithm(float p, int64_t numberOfMessages, int maxNumber = 0, float p_rcp = 0)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(0.f, 1.f);

	int64_t tries = 0;
	int64_t sumOfTries = 0;

	bool receipt = false;
	for (int message = 0; message < numberOfMessages;)
	{
		// "Sending message"
		tries++;

		// Simulating error
		if (dist(rng) <= p)
		{
			// If was error then send "-"
			receipt = false;
		}
		else
		{
			// Simulate error in receipt 
			if (dist(rng) <= p_rcp)
			{
				receipt = false;
			} else
			{
				receipt = true;
			}	
		}

		// Get the receipt
		if (receipt == true || maxNumber != 0 && tries >= maxNumber)
		{
			// Proceed to next message
			message++;
			sumOfTries += tries;
			tries = 0;
		}
	}

	float avgNumOfTries = sumOfTries / (float)numberOfMessages;

	return avgNumOfTries;
}

float UsageInWaitingAlgorithm(float p, float tau, int64_t numberOfMessages)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(0.f, 1.f);

	int64_t tries = 0;

	bool receipt = false;
	bool newMessage = true;
	for (int message = 1; message <= numberOfMessages;)
	{
		// "Sending message"
		if (newMessage)
		{
			std::cout << message;
		}
		else {
			std::cout << " ";
		}
		std::cout << "|\t\t|" << std::endl;
		
		for (int i = 0; i < tau - 2; i++)
			std::cout << " |\t\t|" << std::endl;
		
		std::cout << " |\t";
		
		// Simulating error
		if (dist(rng) <= p)
		{
			// If was error then send "-"
			std::cout << "e_______|x" << std::endl;
			receipt = false;
		}
		else
		{
			std::cout << "ok______|" << message << std::endl;
			receipt = true;
		}

		std::cout << " |______|\t|" << std::endl;
		// Get the receipt
		if (receipt == true)
		{
			// Proceed to next message
			message++;
			newMessage = true;
		}
		else {
			newMessage = false;
		}

		tries++;
	}

	return (numberOfMessages / (float) tries) / (1 + tau);
}

float UsageInReturnAlgorithm(float p, float tau, int64_t numberOfMessages)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_real_distribution<float> dist(0.f, 1.f);

	int64_t tries = 0;
	int64_t cycle = 0;
	int64_t cycleSum = 0;

	bool receipt = false;
	bool wasError = false;
	int  lostMsgNum = 0;
	int  tmpMsg = 0;
	for (int message = 1;;)
	{
		// Simulating error
		if (wasError && lostMsgNum < tau)
		{
			std::cout << message << "|\t";
			std::cout << "~\t|x" << std::endl;
			lostMsgNum++;
			cycle++;
		}
		else
		{
			if (wasError)
			{
				message = tmpMsg;
			}
			std::cout << message << "|\t";

			if (dist(rng) <= p)
			{
				// If was error then send "-"
				std::cout << "e\t|x" << std::endl;
				receipt = false;
				wasError = true;
				lostMsgNum = 0;
				tmpMsg = message;
			}
			else
			{
				cycleSum += cycle++;
				std::cout << "ok\t|" << message << " T: " << cycle << std::endl;

				if (message == numberOfMessages) { break; }
				cycle = 0;
				wasError = false;
				receipt = true;
			}
		}

		message++;
		tries++;
	}

	return (numberOfMessages / (float)tries) / (cycleSum / numberOfMessages);
}

int main()
{
	/* 2.1 Среднее число передач в алгоритме с ожиданием при неограниченном числе повторных передач */
	
	std::cout << "Waiting, unlimited:\t\t" << AvgInWaitingAlgorithm(0.8f, 100000) << std::endl;
	
	/* 2.2 Среднее число передач в алгоритме с ожиданием при ограниченном числе повторных передач */

	std::cout << "Waiting, limited:\t\t" << AvgInWaitingAlgorithm(0.8f, 100000, 2) << std::endl;

	/* 2.3 Среднее число передач в алгоритме с ожиданием при наличии ошибок в обратном канал с неогр. и огр. повт. передач*/

	std::cout << "Waiting, unlimited, p_rcp:\t" << AvgInWaitingAlgorithm(0.8f, 100000, 0, 0.2f) << std::endl;

	std::cout << "Waiting, limited, p_rcp:\t" << AvgInWaitingAlgorithm(0.8f, 100000, 2, 0.2f) << std::endl;

	/* 2.4 Определение коэффициента использования канала при τ > 0 для алгоритма с ожиданием */
	float p4 = 0.5f;
	float tau4 = 2;

	std::cout << "Usage koeff:\t\t" << UsageInWaitingAlgorithm(p4, tau4, 9) << std::endl;
	std::cout << "Should be:\t\t" << (1 - p4) / (1 + tau4) << std::endl;

	/* 2.5 Моделирование алгоритма с возвратом */

	float p5 = 0.4f;
	float tau5 = 3;
	std::cout << "Usage koeff:\t\t" << UsageInReturnAlgorithm(p5, tau5, 1000) << std::endl;
	std::cout << "Should be:\t\t" << (1 - p5) / (1 + p5 * tau5) << std::endl;


}