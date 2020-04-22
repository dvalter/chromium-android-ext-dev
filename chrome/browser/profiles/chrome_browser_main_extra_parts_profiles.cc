// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/profiles/chrome_browser_main_extra_parts_profiles.h"

#include <memory>
#include <utility>

#include "build/build_config.h"
#include "chrome/browser/autocomplete/autocomplete_classifier_factory.h"
#include "chrome/browser/autocomplete/in_memory_url_index_factory.h"
#include "chrome/browser/autocomplete/shortcuts_backend_factory.h"
#include "chrome/browser/autofill/personal_data_manager_factory.h"
#include "chrome/browser/background/background_contents_service_factory.h"
#include "chrome/browser/bookmarks/bookmark_model_factory.h"
#include "chrome/browser/browsing_data/browsing_data_history_observer_service.h"
#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate_factory.h"
#include "chrome/browser/chrome_browser_main.h"
#include "chrome/browser/client_hints/client_hints_factory.h"
#include "chrome/browser/consent_auditor/consent_auditor_factory.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/custom_handlers/protocol_handler_registry_factory.h"
#include "chrome/browser/dom_distiller/dom_distiller_service_factory.h"
#include "chrome/browser/domain_reliability/service_factory.h"
#include "chrome/browser/download/download_core_service_factory.h"
#include "chrome/browser/download/download_service_factory.h"
#include "chrome/browser/engagement/site_engagement_service.h"
#include "chrome/browser/engagement/site_engagement_service_factory.h"
#include "chrome/browser/favicon/favicon_service_factory.h"
#include "chrome/browser/favicon/history_ui_favicon_request_handler_factory.h"
#include "chrome/browser/feature_engagement/tracker_factory.h"
#include "chrome/browser/google/google_search_domain_mixing_metrics_emitter_factory.h"
#include "chrome/browser/history/history_service_factory.h"
#include "chrome/browser/history/top_sites_factory.h"
#include "chrome/browser/invalidation/deprecated_profile_invalidation_provider_factory.h"
#include "chrome/browser/language/language_model_manager_factory.h"
#include "chrome/browser/language/url_language_histogram_factory.h"
#include "chrome/browser/media/history/media_history_keyed_service_factory.h"
#include "chrome/browser/media/media_engagement_service.h"
#include "chrome/browser/media/media_engagement_service_factory.h"
#include "chrome/browser/media/router/media_router_factory.h"
#include "chrome/browser/media/webrtc/webrtc_event_log_manager_keyed_service_factory.h"
#include "chrome/browser/media_galleries/media_galleries_preferences_factory.h"
#include "chrome/browser/notifications/notifier_state_tracker_factory.h"
#include "chrome/browser/ntp_snippets/content_suggestions_service_factory.h"
#include "chrome/browser/password_manager/password_store_factory.h"
#include "chrome/browser/permissions/adaptive_quiet_notification_permission_ui_enabler.h"
#include "chrome/browser/plugins/plugin_prefs_factory.h"
#include "chrome/browser/policy/cloud/user_cloud_policy_invalidator_factory.h"
#include "chrome/browser/predictors/autocomplete_action_predictor_factory.h"
#include "chrome/browser/predictors/loading_predictor_factory.h"
#include "chrome/browser/predictors/predictor_database_factory.h"
#include "chrome/browser/prerender/prerender_link_manager_factory.h"
#include "chrome/browser/prerender/prerender_manager_factory.h"
#include "chrome/browser/prerender/prerender_message_filter.h"
#include "chrome/browser/profiles/gaia_info_update_service_factory.h"
#include "chrome/browser/profiles/renderer_updater_factory.h"
#include "chrome/browser/safe_browsing/certificate_reporting_service_factory.h"
#include "chrome/browser/search/suggestions/suggestions_service_factory.h"
#include "chrome/browser/search_engines/template_url_fetcher_factory.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/send_tab_to_self/send_tab_to_self_client_service_factory.h"
#include "chrome/browser/sessions/tab_restore_service_factory.h"
#include "chrome/browser/sharing/sharing_service_factory.h"
#include "chrome/browser/signin/about_signin_internals_factory.h"
#include "chrome/browser/signin/account_consistency_mode_manager_factory.h"
#include "chrome/browser/signin/account_investigator_factory.h"
#include "chrome/browser/signin/account_reconcilor_factory.h"
#include "chrome/browser/signin/chrome_signin_client_factory.h"
#include "chrome/browser/signin/identity_manager_factory.h"
#include "chrome/browser/signin/signin_profile_attributes_updater_factory.h"
#include "chrome/browser/sync/model_type_store_service_factory.h"
#include "chrome/browser/sync/profile_sync_service_factory.h"
#include "chrome/browser/sync/user_event_service_factory.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/translate/translate_ranker_factory.h"
#include "chrome/browser/ui/app_list/app_list_syncable_service_factory.h"
#include "chrome/browser/ui/find_bar/find_bar_state_factory.h"
#include "chrome/browser/ui/prefs/prefs_tab_helper.h"
#include "chrome/browser/ui/tabs/pinned_tab_service_factory.h"
#include "chrome/browser/ui/webui/ntp/ntp_resource_cache_factory.h"
#include "chrome/browser/ui/webui/signin/login_ui_service_factory.h"
#include "chrome/browser/undo/bookmark_undo_service_factory.h"
#include "chrome/browser/unified_consent/unified_consent_service_factory.h"
#include "chrome/browser/web_data_service_factory.h"
#include "chrome/common/buildflags.h"
#include "components/feature_engagement/buildflags.h"
#include "components/safe_browsing/buildflags.h"
#include "components/spellcheck/spellcheck_buildflags.h"
#include "extensions/buildflags/buildflags.h"
#include "media/base/media_switches.h"
#include "ppapi/buildflags/buildflags.h"
#include "printing/buildflags/buildflags.h"

#if defined(OS_ANDROID)
#include "chrome/browser/android/explore_sites/explore_sites_service_factory.h"
#include "chrome/browser/android/search_permissions/search_permissions_service.h"
#include "chrome/browser/media/android/cdm/media_drm_origin_id_manager_factory.h"

#include "chrome/browser/apps/app_service/app_service_proxy_factory.h"
#include "chrome/browser/feedback/feedback_uploader_factory_chrome.h"
#include "chrome/browser/metrics/desktop_session_duration/desktop_profile_session_durations_service_factory.h"
#include "chrome/browser/resource_coordinator/local_site_characteristics_data_store_factory.h"
#include "chrome/browser/search/instant_service_factory.h"
#include "chrome/browser/storage/storage_notification_service_factory.h"
#include "chrome/browser/ui/global_error/global_error_service_factory.h"
#include "chrome/browser/ui/media_router/media_router_ui_service_factory.h"
#include "chrome/browser/usb/usb_chooser_context_factory.h"
#endif

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/browser_context_keyed_service_factories.h"
#else
#include "chrome/browser/policy/cloud/user_policy_signin_service_factory.h"
#endif

#if defined(OS_WIN)
#include "chrome/browser/profile_resetter/triggered_profile_resetter_factory.h"
#endif

#if BUILDFLAG(ENABLE_CAPTIVE_PORTAL_DETECTION)
#include "chrome/browser/captive_portal/captive_portal_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_LEGACY_DESKTOP_IN_PRODUCT_HELP)
#include "chrome/browser/feature_engagement/bookmark/bookmark_tracker_factory.h"
#include "chrome/browser/feature_engagement/incognito_window/incognito_window_tracker_factory.h"
#include "chrome/browser/feature_engagement/new_tab/new_tab_tracker_factory.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "apps/browser_context_keyed_service_factories.h"
#include "chrome/browser/apps/platform_apps/api/browser_context_keyed_service_factories.h"
#include "chrome/browser/apps/platform_apps/browser_context_keyed_service_factories.h"
#include "chrome/browser/extensions/browser_context_keyed_service_factories.h"
#include "chrome/browser/extensions/extension_management.h"
#include "chrome/browser/ui/web_applications/web_app_metrics_factory.h"
#include "chrome/browser/web_applications/web_app_provider_factory.h"
#include "extensions/browser/browser_context_keyed_service_factories.h"
#endif

#if BUILDFLAG(ENABLE_PRINT_PREVIEW) && !defined(OS_CHROMEOS)
#include "chrome/browser/printing/cloud_print/cloud_print_proxy_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_SERVICE_DISCOVERY)
#include "chrome/browser/printing/cloud_print/privet_notifications_factory.h"
#endif

#if BUILDFLAG(ENABLE_SESSION_SERVICE)
#include "chrome/browser/sessions/session_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_SPELLCHECK)
#include "chrome/browser/spellchecker/spellcheck_factory.h"
#endif

#if BUILDFLAG(ENABLE_SUPERVISED_USERS)
#include "chrome/browser/supervised_user/supervised_user_service_factory.h"
#endif

#if BUILDFLAG(FULL_SAFE_BROWSING)
#include "chrome/browser/safe_browsing/advanced_protection_status_manager_factory.h"
#endif

namespace chrome {

void AddProfilesExtraParts(ChromeBrowserMainParts* main_parts) {
  main_parts->AddParts(new ChromeBrowserMainExtraPartsProfiles());
}

}  // namespace chrome

ChromeBrowserMainExtraPartsProfiles::ChromeBrowserMainExtraPartsProfiles() {}

ChromeBrowserMainExtraPartsProfiles::~ChromeBrowserMainExtraPartsProfiles() {}

// This method gets the instance of each ServiceFactory. We do this so that
// each ServiceFactory initializes itself and registers its dependencies with
// the global PreferenceDependencyManager. We need to have a complete
// dependency graph when we create a profile so we can dispatch the profile
// creation message to the services that want to create their services at
// profile creation time.
//
// TODO(erg): This needs to be something else. I don't think putting every
// FooServiceFactory here will scale or is desirable long term.
//
// static

// Special Android
// When we are on Android, we need to initialize the first pass only the minimum to be able to init the profile the first time
void ChromeBrowserMainExtraPartsProfiles::
    EnsureBrowserContextKeyedServiceFactoriesBuilt(bool full_init) {
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 1";
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (full_init) {
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 2";
  apps::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 3";
  extensions::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 4";
  extensions::ExtensionManagementFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 5";
  chrome_extensions::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 6";
  chrome_apps::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 7";
  chrome_apps::api::EnsureAPIBrowserContextKeyedServiceFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 8";
  }
#endif

#if defined(OS_CHROMEOS)
  chromeos::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 9";
  app_list::AppListSyncableServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 10";

#if !defined(OS_ANDROID)
  apps::AppServiceProxyFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 11";
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 12";
  AboutSigninInternalsFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 13";
  AccountConsistencyModeManagerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 14";
  AccountInvestigatorFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 15";
  AccountReconcilorFactory::GetInstance();
  AdaptiveQuietNotificationPermissionUiEnabler::Factory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 16";
  if (full_init)
  AutocompleteClassifierFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 17";
  autofill::PersonalDataManagerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 18";
#if BUILDFLAG(ENABLE_BACKGROUND_CONTENTS)
  BackgroundContentsServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 19";
  BookmarkModelFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 20";
  BookmarkUndoServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 21";
  BrowsingDataHistoryObserverService::Factory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 22";
  browser_sync::UserEventServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 23";
#if BUILDFLAG(ENABLE_CAPTIVE_PORTAL_DETECTION)
  CaptivePortalServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 24";
  CertificateReportingServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 25";
  if (full_init)
  ChromeBrowsingDataRemoverDelegateFactory::GetInstance();
  ChromeSigninClientFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 28";
  ClientHintsFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 29";
#if BUILDFLAG(ENABLE_PRINT_PREVIEW) && !defined(OS_CHROMEOS)
  CloudPrintProxyServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 30";
  ConsentAuditorFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 31";
  ContentSuggestionsServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 32";
  CookieSettingsFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 33";
  NotifierStateTrackerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 34";
  dom_distiller::DomDistillerServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 35";
  DownloadCoreServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 36";
  DownloadServiceFactory::GetInstance();
#if defined(OS_ANDROID)
  explore_sites::ExploreSitesServiceFactory::GetInstance();
#endif
  FaviconServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 41";
  if (full_init)
  HistoryUiFaviconRequestHandlerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 42";
#if BUILDFLAG(ENABLE_LEGACY_DESKTOP_IN_PRODUCT_HELP)
  feature_engagement::BookmarkTrackerFactory::GetInstance();
  feature_engagement::IncognitoWindowTrackerFactory::GetInstance();
  feature_engagement::NewTabTrackerFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 43";
  feature_engagement::TrackerFactory::GetInstance();
#if !defined(OS_ANDROID)
  feedback::FeedbackUploaderFactoryChrome::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 44";
  FindBarStateFactory::GetInstance();
  GAIAInfoUpdateServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 45";
#if !defined(OS_ANDROID)
  GlobalErrorServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 46";
  GoogleSearchDomainMixingMetricsEmitterFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47";
  HistoryServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-1";
  HostContentSettingsMapFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-2";
  IdentityManagerFactory::EnsureFactoryAndDependeeFactoriesBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-3";
  InMemoryURLIndexFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-4";
  invalidation::DeprecatedProfileInvalidationProviderFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-5";
#if true || !defined(OS_ANDROID)
  InstantServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-6";
#if BUILDFLAG(ENABLE_SERVICE_DISCOVERY)
  cloud_print::PrivetNotificationServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 47-7";
  RendererUpdaterFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 48";
#if BUILDFLAG(ENABLE_SUPERVISED_USERS)
  if (full_init)
  SupervisedUserServiceFactory::GetInstance();
#endif
  LanguageModelManagerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 48-4";
#if !defined(OS_ANDROID)
  LoginUIServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 49";
  if (MediaEngagementService::IsEnabled())
    MediaEngagementServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 50";
  media_router::MediaRouterFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 51";
#if !defined(OS_ANDROID)
  media_router::MediaRouterUIServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 52";
#if defined(OS_ANDROID)
  MediaDrmOriginIdManagerFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 53";
#if !defined(OS_ANDROID)
  MediaGalleriesPreferencesFactory::GetInstance();
#endif
  if (base::FeatureList::IsEnabled(media::kUseMediaHistoryStore))
    media_history::MediaHistoryKeyedServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 54";
#if defined(OS_WIN) || defined(OS_MACOSX) || \
    (defined(OS_LINUX) && !defined(OS_CHROMEOS))
  metrics::DesktopProfileSessionDurationsServiceFactory::GetInstance();
#endif
  ModelTypeStoreServiceFactory::GetInstance();
#if !defined(OS_ANDROID)
  NTPResourceCacheFactory::GetInstance();
#endif
  PasswordStoreFactory::GetInstance();
#if true || !defined(OS_ANDROID)
  PinnedTabServiceFactory::GetInstance();
  ThemeServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 55";
#if BUILDFLAG(ENABLE_PLUGINS)
  PluginPrefsFactory::GetInstance();
#endif
  PrefsTabHelper::GetServiceInstance();
#if !defined(OS_CHROMEOS)
  policy::UserPolicySigninServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58";
  policy::UserCloudPolicyInvalidatorFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-1";
  if (full_init)
  predictors::AutocompleteActionPredictorFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-2";
  if (full_init)
  predictors::PredictorDatabaseFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-3";
  if (full_init)
  predictors::LoadingPredictorFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-4";
  if (full_init)
  prerender::PrerenderLinkManagerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-5";
  if (full_init)
  prerender::PrerenderManagerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-6";
  if (full_init)
  prerender::PrerenderMessageFilter::EnsureShutdownNotifierFactoryBuilt();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-7";
  ProfileSyncServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 58-8";
  ProtocolHandlerRegistryFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 59";
#if !defined(OS_ANDROID)
  resource_coordinator::LocalSiteCharacteristicsDataStoreFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 59-1";
#if BUILDFLAG(FULL_SAFE_BROWSING)
  safe_browsing::AdvancedProtectionStatusManagerFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 60";
#if defined(OS_ANDROID)
  SearchPermissionsService::Factory::GetInstance();
#endif
  send_tab_to_self::SendTabToSelfClientServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 61";
#if BUILDFLAG(ENABLE_SESSION_SERVICE)
  SessionServiceFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 62";
  if (full_init)
  SharingServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 62-1";
  ShortcutsBackendFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 62-2";
  SigninProfileAttributesUpdaterFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 62-3";

  if (SiteEngagementService::IsEnabled())
    SiteEngagementServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 62-4";

#if BUILDFLAG(ENABLE_SPELLCHECK)
  SpellcheckServiceFactory::GetInstance();
#endif
#if !defined(OS_ANDROID)
  StorageNotificationServiceFactory::GetInstance();
#endif
  suggestions::SuggestionsServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 63";
  TabRestoreServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 64-1";
  TemplateURLFetcherFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 64-2";
  TemplateURLServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 64-3";
  TopSitesFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 64-4";
  translate::TranslateRankerFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 65";
#if defined(OS_WIN)
  TriggeredProfileResetterFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 65-1";
  UnifiedConsentServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 65-2";
  UrlLanguageHistogramFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 66";
#if !defined(OS_ANDROID)
  UsbChooserContextFactory::GetInstance();
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 67";
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (full_init) {
  web_app::WebAppProviderFactory::GetInstance();
  web_app::WebAppMetricsFactory::GetInstance();
  }
#endif
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 68";
  WebDataServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 69";
  webrtc_event_logging::WebRtcEventLogManagerKeyedServiceFactory::GetInstance();
  LOG(ERROR) << "[Kiwi] ChromeBrowserMainExtraPartsProfiles::EnsureBrowserContextKeyedServiceFactoriesBuilt - Step 70";
}

void ChromeBrowserMainExtraPartsProfiles::PreProfileInit() {
  EnsureBrowserContextKeyedServiceFactoriesBuilt();
}
