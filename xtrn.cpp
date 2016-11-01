#include<iostream>
#include<fstream>
#include<string>
#include<random>
#include<algorithm>
#include<iterator>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<sstream>

struct Log
{
  Log(void) = delete;

  Log(std::string && filename) : outFile(filename, std::ios::app) {};

  ~Log(void)
  {
    this->outFile.close();
  }

  void operator () (std::string && msg)
  {
    this->outFile << msg << "\n";
  }

  private:
    std::ofstream outFile;
};

struct User
{
  User(void) = delete;

  User(std::string && name) : user_name(name), coin_count(0) {};

  std::string & name(void) 
  {
    return this->user_name;
  }

  int coins(void)
  {
    return coin_count;
  }

  void pay(void)
  {
    coin_count = coin_count + 1;
  }

  int guess(void)
  {
    return std::rand() % 10 + 1;
  }

  private:
    std::string user_name;
    int coin_count;
};

struct CoinServer
{
  CoinServer(void) 
  {
    std::random_shuffle(guess_range.begin(), guess_range.end());
    this->rand_num = guess_range[0];
  };

  bool HandleGuess(User & user, int guess)
  {
    return guess == this->rand_num ? true : false;
  }

  int GetCoins(User & user)
  {
    return user.coins();
  }

  void StartGuessing(void)
  {
    Log log("xternLogFile.txt");

    User user1("user1"), user2("user2");

    log("Created 2 users: `" + user1.name() + "` and `" + user2.name() + "`.\n");
    
    int n = 25;
    do
    {
      std::string rand_num_str;
      std::stringstream ss;
      ss << this->rand_num;
      ss >> rand_num_str;

      log("Current target number: " + rand_num_str);
      turn(log, user1);
      turn(log, user2);
      log("\n");
      n--;
    } while(n > 0);
  }

  void turn(Log & log, User & user)
  {
    if(this->HandleGuess(user, user.guess()))
    {
      user.pay();
      this->regenerate_num();

    }
    logState(log, user);
  }

  void logState(Log & log, User & user)
  {
    std::string user_coins_str;
    std::stringstream ss;
    ss << this->GetCoins(user);
    ss >> user_coins_str;

    log("User: " + user.name() + " has " + user_coins_str + " coins.");
  }

  void regenerate_num(void)
  {
    std::random_shuffle(guess_range.begin(), guess_range.end());
    this->rand_num = guess_range[0];
  }

  private:
    std::vector<int> guess_range = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int rand_num;
};

int main(void)
{
  std::srand(std::time(0));
 
  CoinServer server;

  server.StartGuessing();
  
  return 0;
}
