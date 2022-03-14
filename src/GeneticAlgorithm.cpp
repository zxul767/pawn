#include "GeneticAlgorithm.hpp"
#include "Chromosome.hpp"
#include "FitnessEvaluator.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>

namespace learning
{
using std::vector;

GeneticAlgorithm::GeneticAlgorithm(
    uint population_size, uint iterations_count, double mutation_probability,
    FitnessEvaluator *fitness_evaluator)
{
    this->population_size = population_size;
    this->iterations_count = iterations_count;
    this->mutation_probability = mutation_probability;
    this->fitness_evaluator = fitness_evaluator;
}

void GeneticAlgorithm::run()
{
    initialize_population();

    uint subpopulation_size = (uint)(this->population_size * SELECTION_PERCENTAGE);
    if (util::is_odd(subpopulation_size))
        ++subpopulation_size;

    for (uint i = 0; i < this->iterations_count; ++i)
    {
        evaluate_population();
        set_actual_fitness();

        vector<Chromosome> parents;
        parents.clear();
        select_breeding_individuals(this->population, subpopulation_size, parents);

        vector<Chromosome> offspring;
        offspring.clear();
        uint size = parents.size();
        while (size >= 2)
        {
            std::pair<Chromosome, Chromosome> children;

            uint selected = rand() % size;
            Chromosome individualA = parents[selected];
            parents[selected] = parents[size - 1];
            parents.pop_back();

            selected = rand() % (--size);
            Chromosome individualB = parents[selected];
            parents[selected] = parents[size - 1];
            parents.pop_back();

            individualA.reproduce(individualB, children);
            offspring.push_back(children.first);
            offspring.push_back(children.second);

            --size;
        }

        reduce_population(subpopulation_size);
        add_on_population(offspring);

        uint individuals = rand() % population_size;
        while (individuals)
        {
            population[rand() % this->population_size].mutate(this->mutation_probability);
            --individuals;
        }

        if (converge())
            break;
    }
}

bool GeneticAlgorithm::converge()
{
    return false;
}

void GeneticAlgorithm::reduce_population(uint size)
{
    std::cerr << "Reducing this population . . ." << '\n';

    sort(this->population.begin(), this->population.end());

    for (uint i = 0; i < size; ++i)
        this->population.erase(this->population.begin());

    this->population_size = this->population.size();
}

void GeneticAlgorithm::add_on_population(const vector<Chromosome> &elements)
{
    uint size = elements.size();

    for (uint i = 0; i < size; ++i)
        this->population.push_back(elements[i]);

    this->population_size = population.size();
}

void GeneticAlgorithm::set_seed(Chromosome &seed)
{
    this->fittest_member = seed;
}

Chromosome GeneticAlgorithm::get_fittest_member()
{
    return Chromosome(this->fittest_member);
}

// Initialize the population with random values in their gene strings
// so that we can get a uniform sample of the solution space
void GeneticAlgorithm::initialize_population()
{
    uint features_count = this->fittest_member.features_count();
    vector<int> features;

    for (uint i = 0; i < this->population_size; ++i)
    {
        Chromosome individual(features_count);
        this->population.push_back(individual);
    }
}

void GeneticAlgorithm::evaluate_population()
{
    uint best_index = 0;
    uint average_game_duration = 0;

    for (uint i = 0; i < this->population.size(); ++i)
    {
        double score =
            this->fitness_evaluator->evaluate(this->fittest_member, population[i]);
        this->population[i].set_fitness(score);

        average_game_duration += this->population[i].get_game_duration();

        if (this->fittest_member.get_fitness() < this->population[i].get_fitness())
            best_index = i;
    }
    average_game_duration /= this->population.size();
    this->fittest_member = this->population[best_index];
}

// Use fitness proportionate selection here (aka roulette-wheel selection) to
// a second chance to not-so-good candidate solutions.
void GeneticAlgorithm::select_breeding_individuals(
    vector<Chromosome> &population, uint breeding_population_size,
    vector<Chromosome> &selection)
{
    if (breeding_population_size == 0)
        return;

    double population_fitness = 0.0;

    sort(this->population.begin(), this->population.end());

    for (uint i = 0; i < this->population.size(); ++i)
        population_fitness += this->population[i].get_fitness();

    for (uint i = 0; i < this->population.size(); ++i)
    {
        assert(this->population[i].get_fitness() != 0);
        this->population[i].set_selection_probability(
            this->population[i].get_fitness() / population_fitness);
    }

    double cumulative_probability = 0.0;
    for (uint i = 0; i < this->population.size(); ++i)
    {
        cumulative_probability += this->population[i].get_selection_probability();
        population[i].set_cumulative_probability(cumulative_probability);
    }

    vector<Chromosome> eligible_population = this->population;
    double r = util::Util::random(util::constants::EPSILON, cumulative_probability);

    uint i = 0;
    for (; i < eligible_population.size(); ++i)
    {
        if (r > eligible_population[i].get_cumulative_probability() &&
            i + 1 < eligible_population.size())
            continue;
        else
            break;
    }
    selection.push_back(this->population[i]);
    eligible_population.erase(eligible_population.begin() + i);
    select_breeding_individuals(
        eligible_population, breeding_population_size - 1, selection);
}

void GeneticAlgorithm::set_actual_fitness()
{
    vector<Chromosome> winners, losers, averages;
    this->population_size = this->population.size();

    for (uint i = 0; i < this->population_size; ++i)
        switch (this->population[i].get_result())
        {
        case Chromosome::WIN:
            winners.push_back(this->population[i]);
            break;

        case Chromosome::LOSS:
            losers.push_back(this->population[i]);
            break;

        case Chromosome::DRAW:
            averages.push_back(this->population[i]);
            break;

        default:
            std::cerr << "an unclasified individual";
            abort();
        }

    set_material_fitness(winners);
    set_material_fitness(losers);
    set_material_fitness(averages);
    set_duration_fitness(winners, Chromosome::WIN);
    set_duration_fitness(losers, Chromosome::LOSS);

    this->population.clear();
    uint size = winners.size();
    for (uint i = 0; i < size; ++i)
        this->population.push_back(winners[i]);

    size = losers.size();
    for (uint i = 0; i < size; ++i)
        this->population.push_back(losers[i]);
    size = averages.size();

    for (uint i = 0; i < size; ++i)
        this->population.push_back(averages[i]);

    assert(this->population_size == this->population.size());
}

void GeneticAlgorithm::set_material_fitness(vector<Chromosome> &subpopulation)
{
    uint size = subpopulation.size();

    if (size == 0)
        return;

    vector<int> total;
    for (uint i = 0; i < size; ++i)
        total.push_back(subpopulation[i].get_material_balance());

    sort(total.begin(), total.end());
    int best_result = total.back();

    for (uint i = 0; i < size; ++i)
    {
        double bonus;

        if (subpopulation[i].get_material_balance() != 0)
        {
            double portion =
                (double)best_result / (double)subpopulation[i].get_material_balance();

            if (portion > 1.0)
                portion = 1.0 / portion;

            bonus = portion * (2.0 / 9.0);
        }
        else
        {
            bonus = 0.0;
        }

        assert(bonus <= (2.0 / 9.0));
        subpopulation[i].set_fitness(subpopulation[i].get_fitness() + bonus);
    }
}

void GeneticAlgorithm::set_duration_fitness(
    vector<Chromosome> &subpopulation, Chromosome::Outcome type)
{
    uint size = subpopulation.size();

    if (size == 0)
        return;

    vector<uint> total;
    for (uint i = 0; i < size; ++i)
        total.push_back(subpopulation[i].get_game_duration());

    sort(total.begin(), total.end());
    uint best_result;

    if (type == Chromosome::WIN)
        best_result = total.front();
    else if (type == Chromosome::LOSS)
        best_result = total.back();
    else
    {
        assert(false);
    }

    for (uint i = 0; i < size; ++i)
    {
        assert(subpopulation[i].get_game_duration() != 0);

        double portion =
            (double)best_result / (double)subpopulation[i].get_game_duration();

        if (type == Chromosome::LOSS)
            portion = 1 / portion;

        double bonus = portion * (1.0 / 9.0);

        assert(bonus <= (1.0 / 9.0));
        subpopulation[i].set_fitness(subpopulation[i].get_fitness() + bonus);
    }
}

} // namespace learning
