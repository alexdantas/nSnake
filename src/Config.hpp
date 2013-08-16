#ifndef CONFIG_H_DEFINED
#define CONFIG_H_DEFINED

/// Handles all in-game configuration schemes.
class Config
{
public:
    Config();
    virtual ~Config();

    static bool centerGameScreenVertically;
    static bool centerGameScreenHorizontally;

private:
};

#endif //CONFIG_H_DEFINED

