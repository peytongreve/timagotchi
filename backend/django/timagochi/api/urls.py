from django.urls import include, path
from . import views

urlpatterns = [
    path('course', views.CourseView.as_view()),
    path('major', views.MajorView.as_view()),
    path('others_online', views.OthersOnlineView.as_view()),
    path('get_timagochi', views.TimagochiView.as_view()),
    path('timagochi_exists', views.TimagochiExistsView.as_view()), 
    path('infinite_run', views.InfiniteRunView.as_view())
]
