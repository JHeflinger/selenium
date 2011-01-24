#ifndef WEBDRIVER_IE_ISELEMENTSELECTEDCOMMANDHANDLER_H_
#define WEBDRIVER_IE_ISELEMENTSELECTEDCOMMANDHANDLER_H_

#include "BrowserManager.h"

namespace webdriver {

class IsElementSelectedCommandHandler : public WebDriverCommandHandler {
public:
	IsElementSelectedCommandHandler(void) {
	}

	virtual ~IsElementSelectedCommandHandler(void) {
	}

protected:
	void IsElementSelectedCommandHandler::ExecuteInternal(BrowserManager *manager, std::map<std::string, std::string> locator_parameters, std::map<std::string, Json::Value> command_parameters, WebDriverResponse * response) {
		if (locator_parameters.find("id") == locator_parameters.end()) {
			response->SetErrorResponse(400, "Missing parameter in URL: id");
			return;
		} else {
			std::wstring element_id(CA2W(locator_parameters["id"].c_str(), CP_UTF8));

			BrowserWrapper *browser_wrapper;
			int status_code = manager->GetCurrentBrowser(&browser_wrapper);
			if (status_code != SUCCESS) {
				response->SetErrorResponse(status_code, "Unable to get browser");
				return;
			}

			// The atom is just the definition of an anonymous
			// function: "function() {...}"; Wrap it in another function so we can
			// invoke it with our arguments without polluting the current namespace.
			std::wstring script(L"(function() { return (");

			// Read in all the scripts
			for (int j = 0; IS_SELECTED[j]; j++) {
				script += IS_SELECTED[j];
			}

			// Now for the magic and to close things
			script += L")})();";

			ElementWrapper *element_wrapper;
			status_code = this->GetElement(manager, element_id, &element_wrapper);
			if (status_code == SUCCESS) {
				bool is_selected = element_wrapper->IsSelected();
				response->SetResponse(SUCCESS, is_selected);
			} else {
				response->SetErrorResponse(status_code, "Element is no longer valid");
				return;
			}
		}
	}
};

} // namespace webdriver

#endif // WEBDRIVER_IE_ISELEMENTSELECTEDCOMMANDHANDLER_H_
