/*
  ==============================================================================

    JobQueue.cpp
    Created: 27 May 2018 8:00:22am
    Author:  paul

  ==============================================================================
*/

#include "JobQueue.h"

Queue::Queue(int count) : Thread("Worker " + count)
{
}

void Queue::AddJob(Job job)
{
  this->jobs.emplace_back(job);
}

void Queue::run()
{
  while (!this->threadShouldExit())
  {
    this->lock.enterRead();
    if (this->jobs.size() == 0)
    {
      this->lock.exitRead();
      continue;
    }

    this->lock.enterWrite();
    auto job = this->jobs[0];
    this->jobs.erase(this->jobs.begin());
    this->lock.exitWrite();
    this->lock.exitRead();

    if (job.Action == "Test")
    {
      this->metrics.emplace_back(
          Metric(
              job.ToTest->Timbre.GetMetric(*job.Amplitude) + job.ToTest->Panning.GetMetric(*job.Panning),
              *job.ToTest));
      continue;
    }

    if (job.Action == "Breed")
    {
      this->candidates.emplace_back(
          Candidate(
              Biology::Gene(job.Mother->Timbre, job.Father->Timbre),
              Biology::Gene(job.Mother->Panning, job.Father->Panning)));
      continue;
    }

    if (job.Action == "Mutate")
    {
      job.ToMutate->Timbre.Mutate(*job.MutationAmount, *job.FrequencyWeighting);
      job.ToMutate->Panning.Mutate(*job.MutationAmount);
      continue;
    }
  }
}

bool Queue::Waiting() const
{
  this->lock.enterRead();
  auto result = this->jobs.size() == 0;
  this->lock.exitRead();
  return result;
}

std::vector<Metric> *Queue::GetMetrics()
{
  return &this->metrics;
}

std::vector<Candidate> *Queue::GetCandidates()
{
  return &this->candidates;
}

void Queue::Clear()
{
  this->metrics.clear();
  this->candidates.clear();
}

JobPool::JobPool(int threads)
{
  for (auto i = 0; i < threads; i++)
  {
    this->queues.emplace_back(new Queue(i));
  }
}

void JobPool::run()
{
  for (auto &queue : this->queues)
  {
    queue->startThread();
  }
}

void JobPool::Kill()
{
  for (auto &queue : this->queues)
  {
    queue->waitForThreadToExit(100);
  }
}

void JobPool::AddTestJob(Candidate *toTest,
                         std::vector<double> *amplitude,
                         std::complex<double> *panning)
{
  Job job{
      .Action = "Test",
      .ToTest = toTest,
      .Mother = nullptr,
      .Father = nullptr,
      .ToMutate = nullptr,
      .Amplitude = amplitude,
      .Panning = panning};
  this->queues[this->current]->AddJob(job);
  this->current++;
  if (this->current >= this->queues.size())
  {
    this->current = 0;
  }
}

void JobPool::AddBreedJob(Metric *mother,
                          Metric *father)
{
  Job job{
      .Action = "Breed",
      .ToTest = nullptr,
      .Mother = mother,
      .Father = father};
  this->queues[this->current]->AddJob(job);
  this->current++;
  if (this->current >= this->queues.size())
  {
    this->current = 0;
  }
}

void JobPool::AddMutationJob(Candidate *toMutate,
                             double *timbreAmount,
                             double *panningAmount,
                             std::vector<double> *weighting)
{
  Job job{
      .Action = "Mutate",
      .ToTest = nullptr,
      .Mother = nullptr,
      .Father = nullptr,
      .ToMutate = toMutate,
      .Amplitude = nullptr,
      .Panning = nullptr,
      .MutationAmount = timbreAmount,
      .PanningMutationAmount = panningAmount,
      .FrequencyWeighting = weighting};
  this->queues[this->current]->AddJob(job);
  this->current++;
  if (this->current >= this->queues.size())
  {
    this->current = 0;
  }
}

SortedSet<Metric> JobPool::GetMetrics()
{
  this->WaitForFinish();
  SortedSet<Metric> result;
  for (auto &queue : this->queues)
  {
    auto output = queue->GetMetrics();
    for (auto &metric : *output)
    {
      result.add(metric);
    }
  }

  return result;
}

std::vector<Candidate> JobPool::GetCandidates()
{
  this->WaitForFinish();
  std::vector<Candidate> result;
  for (auto &queue : this->queues)
  {
    auto output = queue->GetCandidates();
    for (auto &candidate : *output)
    {
      result.emplace_back(candidate);
    }
  }

  return result;
}

void JobPool::WaitForMutation() const
{
  this->WaitForFinish();
}

void JobPool::WaitForFinish() const
{
  while (true)
  {
    auto done = true;
    for (const auto &queue : this->queues)
    {
      done = queue->Waiting() && done;
    }

    if (done)
    {
      return;
    }
  }
}

void JobPool::Clear()
{
  for (auto &queue : this->queues)
  {
    queue->Clear();
  }
}
