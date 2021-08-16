#include "broadcast.h"
#include "equalizer.h"
#include "player.h"
#include "record.h"

// TODO: Reduce amount of ugly global variables
std::unique_ptr<Players> g_players = std::make_unique<Players>();
std::unique_ptr<Equalizers> g_equalizers = std::make_unique<Equalizers>();
std::unique_ptr<Broadcasts> g_broadcasts = std::make_unique<Broadcasts>();
std::unique_ptr<Records> g_records = std::make_unique<Records>();