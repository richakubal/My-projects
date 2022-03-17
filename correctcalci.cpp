#include <iostream>
#include <sstream>
#include <list>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <stdlib.h>
       
const int LEFT_ASSOC  = 0;    
const int RIGHT_ASSOC = 1; 
         
typedef std::map< std::string, std::pair< int,int >> OpMap;   
typedef std::vector<std::string>::const_iterator cv_iter;
typedef std::string::iterator s_iter;
 
const OpMap::value_type assocs[] =   
    {  OpMap::value_type( "+", std::make_pair<int,int>( 0, LEFT_ASSOC ) ),    
       OpMap::value_type( "-", std::make_pair<int,int>( 0, LEFT_ASSOC ) ),      
       OpMap::value_type( "*", std::make_pair<int,int>( 5, LEFT_ASSOC ) ),      
       OpMap::value_type( "/", std::make_pair<int,int>( 5, LEFT_ASSOC ) ) };  
 
const OpMap opmap( assocs, assocs + sizeof( assocs ) / sizeof( assocs[ 0 ] ) );  
   
bool isParenthesis( const std::string& token)        
{        
    return token == "(" || token == ")";      
}      
        
bool isOperator( const std::string& token)        
{        
    return token == "+" || token == "-" ||      
           token == "*" || token == "/";      
}      
         
bool isAssociative( const std::string& token, const int& type)        
{            
    const std::pair<int,int> p = opmap.find( token )->second;        
    return p.second == type;        
}        
         
int cmpPrecedence( const std::string& token1, const std::string& token2 )        
{        
    const std::pair<int,int> p1 = opmap.find( token1 )->second;      
    const std::pair<int,int> p2 = opmap.find( token2 )->second;      
 
    return p1.first - p2.first;        
}        
         
bool infixToRPN( const std::vector<std::string>& inputTokens,   
                 const int& size,   
                 std::vector<std::string>& strArray )        
{     
    bool success = true;    
 
    std::list<std::string> out;      
    std::stack<std::string> stack;      
     
    for ( int i = 0; i < size; i++ )        
    {         
        const std::string token = inputTokens[ i ];      
 
           
        if ( isOperator( token ) )  
        {                    
            const std::string o1 = token;        
 
            if ( !stack.empty() )    
            {    
                std::string o2 = stack.top();    
 
                while ( isOperator( o2 ) &&        
                        ( ( isAssociative( o1, LEFT_ASSOC ) &&  cmpPrecedence( o1, o2 ) == 0 ) ||    
                        ( cmpPrecedence( o1, o2 ) < 0 ) ) )    
                {                               
                    stack.pop();      
                    out.push_back( o2 );                            
 
                    if ( !stack.empty() )     
                        o2 = stack.top();      
                    else   
                        break;    
                }         
            }    
 
            stack.push( o1 );        
        }            
        else if ( token == "(" )        
        {              
            stack.push( token );        
        }               
        else if ( token == ")" )        
        {          
            std::string topToken  = stack.top();  
 
            while ( topToken != "(" )        
            {                        
                out.push_back(topToken );        
                stack.pop();    
 
                if ( stack.empty() ) break;  
                topToken = stack.top();  
            }                                        
                               
            if ( !stack.empty() ) stack.pop();                             
                 
            if ( topToken != "(" )    
            {                        
                return false;    
            }                                          
        }              
        else       
        {        
            out.push_back( token );        
        }        
    }      
     
    while ( !stack.empty() )        
    {      
        const std::string stackToken = stack.top();    
     
        if ( isParenthesis( stackToken )   )  
        {    
            return false;    
        }    
 
        out.push_back( stackToken );                      
        stack.pop();      
    }        
 
    strArray.assign( out.begin(), out.end() );  
 
    return success;    
}        
 
 
double RPNtoDouble( std::vector<std::string> tokens )        
{        
    std::stack<std::string> st;       
         
    for ( int i = 0; i < (int) tokens.size(); ++i )        
    {       
        const std::string token = tokens[ i ];      
         
        if ( !isOperator(token) )        
        {        
            st.push(token);        
        }        
        else       
        {  
            double result =  0.0;  
         
            const std::string val2 = st.top();      
            st.pop();      
            const double d2 = strtod( val2.c_str(), NULL );        
 
            if ( !st.empty() )  
            {  
                const std::string val1 = st.top();      
                st.pop();      
                const double d1 = strtod( val1.c_str(), NULL );     
         
                result = token == "+" ? d1 + d2 :        
                         token == "-" ? d1 - d2 :        
                         token == "*" ? d1 * d2 :        
                                        d1 / d2;        
            }  
            else 
            {  
                if ( token == "-" )  
                    result = d2 * -1;  
                else  
                    result = d2;  
            }  
 
        
            std::ostringstream s;      
            s << result;      
            st.push( s.str() );        
        }        
    }                
 
    return strtod( st.top().c_str(), NULL );      
}        
 
std::vector<std::string> getExpressionTokens( const std::string& expression )  
{  
    std::vector<std::string> tokens;        
    std::string str = "";    
 
    for ( int i = 0; i < (int) expression.length(); ++i )    
    {    
        const std::string token( 1, expression[ i ] );    
 
        if ( isOperator( token ) || isParenthesis( token ) )    
        {
            if ( !str.empty() )
            {
               tokens.push_back( str ) ;
            }
            str = "";               
            tokens.push_back( token );              
        }    
        else   
        {       
    
            if ( !token.empty() && token != " " )    
            {                   
                str.append( token );    
            }    
            else    
            {    
                if ( str != "" )    
                {    
                    tokens.push_back( str );        
                    str = "";    
                }    
            }                           
        }    
    }        
 
    return tokens;  
}
 
template<typename T, typename InputIterator>
void Print( const std::string& message,
             const InputIterator& itbegin,   
             const InputIterator& itend,   
             const std::string& delimiter)  
{  
    std::cout << message << std::endl;
 
    std::copy(itbegin,   
              itend,   
              std::ostream_iterator<T>(std::cout, delimiter.c_str()));  
 
    std::cout << std::endl;
}  
 
int main()        
{       
    std::string s = "( 1 + 2) * ( 3 / 4 )-(5+6)";          
 
    Print<char, s_iter>( "Input expression:", s.begin(), s.end(), "" );                
        
    std::vector<std::string> tokens = getExpressionTokens( s );                           
   
    std::vector<std::string> rpn;        
    if ( infixToRPN( tokens, tokens.size(), rpn ) )      
    {      
        double d = RPNtoDouble( rpn );      
        Print<std::string, cv_iter>( "RPN tokens:  ", rpn.begin(), rpn.end(), " " );
 
        std::cout << "Result = " << d << std::endl;      
    }      
    else     
    {      
        std::cout << "Mis-match in parentheses" << std::endl;      
    }         
 
    return 0;        
} 