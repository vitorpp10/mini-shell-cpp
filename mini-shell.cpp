#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<cstdlib>

int main()
{
  std::string name;

  while (true)
  {
    while (waitpid(-1, NULL, WNOHANG) > 0) { }

    std::cout << "user@host> ";
    std::getline(std::cin, name);

    if (!std::cin)
    {
      std::cout << '\n';
      break;
    }

    if (name == "")
    {
      continue;
    }

    std::vector<std::string> args;
    std::istringstream string_(name);
    std::string token;

    while(string_ >> token)
    {
      args.push_back(token);
    }

    if (args.empty()) { continue; }

    bool is_background{false};

    std::string final_args = args.back();

    if (final_args == "&")
    {
      is_background = true;
      args.pop_back();
    }

    if (args.empty()) { continue; }

    int size_args = args.size();
    std::vector<char*> c_args;  

    if (args[0] == "exit")
    {
      break;
    }

    if (args[0] == "cd")
    {
      if (size_args < 2)
      {
        std::cout << "Esta faltando diretorios. " << std::endl;
      }
      else 
      {
        const char* convert = args[1].c_str();
        int chdir_ = chdir(convert);
        if (chdir_ < 0) { std::cout << "Esta faltando diretorios ou permissoes. " << std::endl; }
      }
      continue;
    }

    for (int i = 0; i < size_args; i++)
    {
      const char* convert = args[i].c_str();
      char* ptr = const_cast<char*>(convert);
      c_args.push_back(ptr);
    }

    c_args.push_back(nullptr);
    
    pid_t pid = fork();
    
    if (pid < 0) { std::cout << "Erro no pid. " << std::endl; continue; }

    if (pid == 0)
    {
      std::cout << "Executando comando pelo processo filho. " << '\n';
      int exe = execve(c_args[0], c_args.data(), NULL);
      if (exe < 0) 
      { 
        std::cout << "Erro, fechando processo. " << std::endl; 
        exit(1);
      }
    }    else 
    {
      if (is_background == false)
      {
        waitpid(pid, NULL, 0);
      }
      else 
      {
        std::cout << "Processo em background, PID: " << pid << std::endl; 
      }
    }
  } 
  return 0;
}
