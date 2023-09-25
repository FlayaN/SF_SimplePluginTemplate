void MessageHandler(SFSE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case SFSE::MessagingInterface::kPostLoad:
		{
			logger::info("{:*^50}", "POST LOAD"sv);
		}
		break;
	default:
		break;
	}
}

DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* a_sfse)
{
#ifndef NDEBUG
	while (!SFSE::WinAPI::IsDebuggerPresent()) {};
#endif
	SFSE::Init(a_sfse);

	auto version = Plugin::Version.string();
	logger::info("Loaded {} {}", Plugin::Name, version);

	const auto messaging = SFSE::GetMessagingInterface();
	messaging->RegisterListener(MessageHandler);

	return true;
}
