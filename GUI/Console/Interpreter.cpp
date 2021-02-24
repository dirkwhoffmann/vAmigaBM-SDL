// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"
#include "Interpreter.h"

Interpreter::Interpreter(Application &ref) : GUIComponent(ref)
{
    registerInstructions();
};

Arguments
Interpreter::split(const string& userInput)
{
    std::stringstream ss(userInput);
    Arguments result;

    std::string token;
    bool str = false;
    
    for (isize i = 0; i < userInput.size(); i++) {

        // Switch between string mode and non-string mode if '"' is detected
        if (userInput[i] == '"') { str = !str; continue; }
        
        // Process character
        if (userInput[i] != ' ' || str) {
            token += userInput[i];
        } else {
            if (!token.empty()) result.push_back(token);
            token = "";
        }
    }
    if (!token.empty()) result.push_back(token);
    
    /*
    printf("Tokens:\n");
    for (auto &it : result) {
        printf("%s\n", it.c_str());
    }
    */
    return result;
}
    
void
Interpreter::autoComplete(Arguments &argv)
{
    Command *current = &root;
    std::string prefix, token;

    for (auto it = argv.begin(); current && it != argv.end(); it++) {
        
        current->autoComplete(*it);
        current = current->seek(*it);
    }
}

void
Interpreter::autoComplete(string& userInput)
{
    // Split input string
    Arguments tokens = split(userInput);
    
    printf("Tokens:\n");
    for (auto &it : tokens) printf("%s\n", it.c_str());
    
    // Complete all tokens
    autoComplete(tokens);

    printf("Completed tokens:\n");
    for (auto &it : tokens) printf("%s\n", it.c_str());

    // Recreate the command string
    userInput = "";
    for (const auto &it : tokens) {
        userInput += (userInput == "" ? "" : " ") + it;
    }

    // Add a space if the command has been fully completed
    if (userInput != "" && root.seek(tokens) != nullptr) {
        userInput += " ";
    }
}

void
Interpreter::exec(const string& userInput, bool verbose)
{
    // Split the command string
    Arguments tokens = split(userInput);
        
    // Auto complete the token list
    autoComplete(tokens);
            
    // Process the command
    exec(tokens, verbose);
}

void
Interpreter::exec(Arguments &argv, bool verbose)
{
    Command *current = &root;
    std::string token;

    // In 'verbose' mode, print the token list
    if (verbose) {
        for (const auto &it : argv) console << it << ' ';
        console << '\n';
    }
    
    // Skip empty lines
    if (argv.empty()) return;
    
    // Seek the command in the command tree
    while (current) {
        
        // Extract token
        token = argv.empty() ? "" : argv.front();
        
        // Break the loop if this token is unknown
        Command *next = current->seek(token);
        if (next == nullptr) break;
        
        // Move one level down
        current = next;
        if (!argv.empty()) argv.pop_front();
    }
        
    // Error out if no command handler is present
    if (current->func == nullptr && !argv.empty()) {
        throw ParseError(token);
    }
    if (current->func == nullptr && argv.empty()) {
        throw TooFewArgumentsError(current->tokens());
    }
    
    // Check the argument count
    if (argv.size() < current->numArgs) throw TooFewArgumentsError(current->tokens());
    if (argv.size() > current->numArgs) throw TooManyArgumentsError(current->tokens());
    
    // Call the command handler
    (controller.*(current->func))(argv, current->param);
}

void
Interpreter::usage(Command& current)
{
    console << "Usage: " << current.usage() << '\n' << '\n';
}

void
Interpreter::help(const string& userInput)
{
    // Split the command string
    Arguments tokens = split(userInput);
        
    // Auto complete the token list
    autoComplete(tokens);
            
    // Process the command
    help(tokens);
}

void
Interpreter::help(Arguments &argv)
{
    Command *current = &root;
    std::string prefix, token;
    
    while (1) {
                
        // Extract token
        token = argv.empty() ? "" : argv.front();
        
        // Check if this token matches a known command
        Command *next = current->seek(token);
        if (next == nullptr) break;
        
        prefix += next->token + " ";
        current = next;
        if (!argv.empty()) argv.pop_front();
    }

    help(*current);
}

void
Interpreter::help(Command& current)
{
    // Print the usage string
    usage(current);
    
    // Collect all argument types
    auto types = current.types();

    // Determine tabular positions to align the output
    int tab = 0;
    // for (auto &it : types) tab = std::max(tab, (int)it.length());
    for (auto &it : current.args) {
        tab = std::max(tab, (int)it.token.length());
        tab = std::max(tab, 2 + (int)it.type.length());
    }
    tab += 5;
    
    for (auto &it : types) {
        
        auto opts = current.filterType(it);
        int size = (int)it.length();

        console.tab(tab - size);
        console << "<" << it << "> : ";
        console << (int)opts.size() << (opts.size() == 1 ? " choice" : " choices");
        console << '\n' << '\n';
        
        for (auto &opt : opts) {

            string name = opt->token == "" ? "<>" : opt->token;
            console.tab(tab + 2 - (int)name.length());
            console << name;
            console << " : ";
            console << opt->info;
            console << '\n';
        }
        console << '\n';
    }
}
