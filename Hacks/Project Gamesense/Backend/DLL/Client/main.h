#pragma once

#include "ActiveSocket.h"
#include "PassiveSocket.h"
#include "SimpleSocket.h"
#include "Host.h"
#include "Client.h"
#include "StatTimer.h"
#include "Xor.h"
#include "sha1.hpp"
#include "../../Security/md5.h"
#include "../../Security/base64.h"
#include "../../Hashing/xorstr.h"
#include "../HWID/HWID.h"

#include <vector>
#include <chrono>

#define On true
#define Off false